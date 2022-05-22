#include "Application.h"
#include<string>
#include<memory>

// 静的変数
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
	// 解放処理
	Input::GetInstance()->Destroy();
	Camera::Destroy();
	SceneManager::GetInstance()->Destroy();
	Xinput::GetInstance()->Destroy();
	FrameFixed::GetInstance()->Destroy();

	// instanceの解放
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Application::Update()
{
	// フレームレート固定
	FrameFixed::GetInstance()->PreWait();

	// 更新
	Input::GetInstance()->Update();
	Xinput::GetInstance()->Update();

	ObjectManager::GetInstance()->Update();

	// 各シーンのupdate
	SceneManager::GetInstance()->Update();

	Draw();

	// フレームレート固定用
	FrameFixed::GetInstance()->PostWait();
}

void Application::Draw()
{
	// 描画準備用
	directX.get()->PreDraw();

	
	SceneManager::GetInstance()->Draw();
	// カメラのデバッグ用
	//Camera::DebugDraw();

	// 描画コマンド消化用
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
	// デバッグレイヤーをオン　初期化
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
	// オブジェクトマネージャー初期化
	ObjectManager::Initialize(directX.get());
}

void Application::ParticleInitialize()
{
	// particle関係
	Particle2D::Init(directX.get());
	ParticleManager::Init(directX.get());
}
