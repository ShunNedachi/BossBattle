#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include"FrameFixed.h"

bool MessageError(MSG msg);


// windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

	#pragma region ����������

	// �E�B���h�E�p
	MyWindow myw;
	myw.Initialize("Engine");

	// DirectInput,Xinput�p������
	Input* input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance,myw.GetHWND());
	Xinput* xinput = Xinput::GetInstance();
	Xinput::Initialize();

	// directX12��{������
	MyDirectX12 my12;
	my12.Debug();
	my12.Initialize(&myw);

	// �I�u�W�F�N�g�}�l�[�W���[������
	ObjectManager::Initialize(&my12);

	// �V�[���Ǘ��p
	SceneManager* sceneManager = SceneManager::GetInstance();

	// fps�Œ�p
	FrameFixed* frameFixed = FrameFixed::GetInstance();

#pragma endregion


	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };

		// �t���[�����[�g�Œ�
		frameFixed->PreWait();

		// �X�V
		input->Update();
		xinput->Update();

		// exe�I���p
		if (xinput->TriggerButtom(0, xinput_BACK))break;

		// �e�V�[����update
		sceneManager->Update();


		#pragma region �`�揈��

		// �`�揀���p
		my12.PreDraw();

		sceneManager->Draw();

		// �`��R�}���h�����p
		my12.PostDraw();

		#pragma endregion

		// fps�Œ�p
		frameFixed->PostWait();
	}


	// �������
	input->Destroy();
	Camera::Destroy();
	sceneManager->Destroy();
	xinput->Destroy();
	frameFixed->Destroy();

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