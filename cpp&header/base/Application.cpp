#include "Application.h"
#include<string>
#include<memory>

// �ÓI�ϐ�
//MyWindow Application::myw;
//MyDirectX12 Application::my12;
//Input* Application::input;
//Xinput* Application::xinput;
//SceneManager* Application::sceneManager;
//FrameFixed* Application::frameFixed;
Application* Application::instance = nullptr;

Application* Application::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Application();
	}

	return instance;
}

void Application::Initialize()
{
	WindowInitialize();

	DXInitialize();

	InputInitialize();

	ManagerInitialize();

	ParticleInitialize();
}

void Application::Destroy()
{
	// �������
	Input::GetInstance()->Destroy();
	Camera::Destroy();
	SceneManager::GetInstance()->Destroy();
	Xinput::GetInstance()->Destroy();
	FrameFixed::GetInstance()->Destroy();

	// instance�̉��
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Application::Update()
{
	// �t���[�����[�g�Œ�
	FrameFixed::GetInstance()->PreWait();

	// �X�V
	Input::GetInstance()->Update();
	Xinput::GetInstance()->Update();

	ObjectManager::GetInstance()->Update();

	// �e�V�[����update
	SceneManager::GetInstance()->Update();

	Draw();

	// �t���[�����[�g�Œ�p
	FrameFixed::GetInstance()->PostWait();
}

void Application::Draw()
{
	// �`�揀���p
	directX.get()->PreDraw();

	
	SceneManager::GetInstance()->Draw();
	// �J�����̃f�o�b�O�p
	//Camera::DebugDraw();

	// �`��R�}���h�����p
	directX.get()->PostDraw();
}

void Application::WindowInitialize()
{
	window = std::make_shared<MyWindow>();
	
	window.get()->Initialize(ApplicationSetting::WINDOW_NAME.c_str());
}

void Application::DXInitialize()
{
	directX = std::make_shared<MyDirectX12>();
	// �f�o�b�O���C���[���I���@������
	directX.get()->Debug();
	directX.get()->Initialize(window.get());
}

void Application::InputInitialize()
{
	Input::GetInstance()->Initialize(window.get()->GetConfig().hInstance, window.get()->GetHWND());
	Xinput::GetInstance()->Xinput::Initialize();
}

void Application::ManagerInitialize()
{
	// �I�u�W�F�N�g�}�l�[�W���[������
	ObjectManager::Initialize(directX.get());
}

void Application::ParticleInitialize()
{
	// particle�֌W
	Particle2D::Init(directX.get());
	ParticleManager::Init(directX.get());
}
