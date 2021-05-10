#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include<wrl.h>
#include"Sprite2D.h"
#include"Object.h"



using namespace Microsoft::WRL;


bool MessageError(MSG msg);

// windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

	#pragma region ����������
	// �E�B���h�E�p
	MyWindow myw;
	myw.Initialize("Engine");

	Input input;
	input.Initialize(myw.GetConfig().hInstance,myw.GetHWND());

	// DirectX ���������� ��������
	MyDirectX12 my12;
	HRESULT result;

	my12.Debug();
	my12.Initialize(&myw);

	Sprite2D::Init(&my12, &myw);

	// DirectX ���������� �����܂�

#pragma endregion



	// �`�揉����


	// �X�v���C�g
	Sprite2D titleSprite(0.5f,0.5f);

	Sprite2D::LoadTex(0, L"Resources/texture/title.png");

	titleSprite.CreateSprite(0);
    titleSprite.SetPosition({ 650,600,0 });


	// �@3D�I�u�W�F�N�g
	Object* obj = new Object();
	obj->Init(&my12, &myw);
	obj->CreateModel("player");

	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };

		// �X�V
		input.Update();

	
		obj->Update({ 0,0,0 });
		#pragma region �`�揈��

		// �`�揀���p
		my12.PreDraw();

		obj->Draw();

		// �`��R�}���h�����p
		my12.PostDraw();

		#pragma endregion

	}

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