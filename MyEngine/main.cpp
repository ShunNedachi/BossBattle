#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include"ObjectManager.h"


bool MessageError(MSG msg);

// windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

#pragma region ����������
	// �E�B���h�E�p
	MyWindow myw;
	myw.Initialize("Engine");

	Input* input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance,myw.GetHWND());

	Xinput xinput;

	// DirectX ���������� ��������
	MyDirectX12 my12;

	my12.Debug();
	my12.Initialize(&myw);



	// DirectX ���������� �����܂�

#pragma endregion

	Camera* camera = Camera::GetInstance();

	SceneManager* sceneManager = SceneManager::GetInstance();


	// �`�揉����
	ObjectManager* objManager;
	objManager = ObjectManager::GetInstance();
	objManager->Initialize(&my12, &myw, camera);

	// �@3D�I�u�W�F�N�g
	objManager->AddOBJ("bless");
	objManager->AddOBJ("bless", { 0,0,-50 });

	// �X�v���C�g
	objManager->AddSprite(0);

	int objNum = 2;

	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };

		// �X�V
		input->Update();
		xinput.Update();


		if (input->TriggerKey(DIK_1))
		{
			sceneManager->NextScene();
		}

		if (input->TriggerKey(DIK_2))
		{
			objNum--;
			objManager->DeleteOBJ(0);
		}

		if (input->TriggerKey(DIK_3))
		{
			objNum++;
			objManager->AddOBJ("bless", {0,0,-50 * (float)objNum});
		}

		// �e�V�[����update
		// ���݂͎g�p���Ă��Ȃ����A��X�q�V�[���N���X�ɏ������ڍs
		sceneManager->Update();

		objManager->Update();
	
		camera->Update();

		#pragma region �`�揈��

		// �`�揀���p
		my12.PreDraw();


		objManager->Draw();

		// �`��R�}���h�����p
		my12.PostDraw();

		#pragma endregion

	}


	// �������
	input->Destroy();
	camera->Destroy();
	sceneManager->Destroy();
	objManager->Destroy();

	return 0;
}

bool MessageError(MSG msg)
{
	// ���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}
	// �I�����b�Z�[�W�������烋�[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	// 0�̂Ƃ����퓮��
	return false;
}