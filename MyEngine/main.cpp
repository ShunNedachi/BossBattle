#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Object.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"


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

	Sprite2D::Init(&my12, &myw);

	Camera* camera = Camera::GetInstance();

	SceneManager* sceneManager = SceneManager::GetInstance();

	// DirectX ���������� �����܂�

#pragma endregion



	// �`�揉����


	// �X�v���C�g
	Sprite2D titleSprite(0.5f,0.5f);

	Sprite2D::LoadTex(0, L"Resources/texture/title.png");

	titleSprite.CreateSprite(0);
    titleSprite.SetPosition({ 650,600,0 });


	// �@3D�I�u�W�F�N�g
	Object* obj = new Object(NORMAL);
	obj->Init(&my12, &myw,camera);
	obj->CreateModel("bless");

	Object* obj2 = new Object(TOON);
	obj2->CreateModel("bless");
	

	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };

		// �X�V
		input->Update();
		xinput.Update();

		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 target = camera->GetTarget();

		if (input->PushKey(DIK_W) || xinput.MoveStick(0,xinput_LS) & XINPUT_STICK_UP)
		{
			 if(eye.z < -1)eye.z++;
		}
		else if (input->PushKey(DIK_S)|| xinput.MoveStick(0,xinput_LS)& XINPUT_STICK_DOWN )
		{
			eye.z--;
		}

		if (input->TriggerKey(DIK_1))
		{
			sceneManager->NextScene();
		}
	
		camera->SetEye(eye);


		obj->Update({ 0,0,0 });
		obj2->Update({ 0,1,0 });
		#pragma region �`�揈��

		// �`�揀���p
		my12.PreDraw();

		//obj->Draw(NORMAL);
		obj->Draw();

		// �`��R�}���h�����p
		my12.PostDraw();

		#pragma endregion

	}

	delete obj;
	delete obj2;

	input->Destroy();
	camera->Destroy();
	sceneManager->Destroy();

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