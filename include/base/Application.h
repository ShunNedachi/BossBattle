#pragma once
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include"FrameFixed.h"
#include"Light.h"
#include"PostEffect.h"

#include"ParticleManager.h"
#include"Particle2D.h"

namespace ApplicationSetting
{
	const std::string WINDOW_NAME = "BOSS BATTLE";
}

// �V���O���g���p�^�[��
class Application
{
public:

	static Application* GetInstance();

	void Initialize();
	void Destroy();

	void Update();

	void Draw();


private:

	// �R���X�g���N�^�@�f�X�g���N�^
	Application() = default;
	~Application() = default;

	void operator=(const Application& obj) = delete;
	Application(const Application& obj) = delete;

	// �E�B���h�E�N���X�̏�����
	void WindowInitialize();
	// directX�N���X�̏�����
	void DXInitialize();
	// �C���v�b�g�N���X�̏�����
	void InputInitialize();
	// �}�l�[�W���[�N���X�̏�����
	void ManagerInitialize();
	// �p�[�e�B�N���N���X�̏�����
	void ParticleInitialize();

public:

	static Application* instance;

	// �E�B���h�E�p
	std::shared_ptr<MyWindow> window;
	// directX12
	std::shared_ptr<MyDirectX12> directX;
	

	std::shared_ptr<std::string> windowName;
};

