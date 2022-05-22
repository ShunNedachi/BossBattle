#include"Application.h"
#include"Input.h"
#include "XinputControll.h"

bool MessageError(MSG msg);


// windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::GetInstance()->Initialize();


	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };


		// exe�I���p
		if (Xinput::GetInstance()->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_BACK)break;

		Application::GetInstance()->Update();

	}
	
	Application::GetInstance()->Destroy();

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

