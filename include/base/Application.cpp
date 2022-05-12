#include "Application.h"

// 静的変数
MyWindow Application::myw;
MyDirectX12 Application::my12;
Input* Application::input;
Xinput* Application::xinput;
SceneManager* Application::sceneManager;
FrameFixed* Application::frameFixed;

void Application::Initialize()
{
	// ウィンドウ初期化
	myw.Initialize("Engine");

	// DirectX12初期化
	my12.Debug();
	my12.Initialize(&myw);

	// input xinput
	input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance, myw.GetHWND());
	xinput = Xinput::GetInstance();
	Xinput::Initialize();

	// オブジェクトマネージャー初期化
	ObjectManager::Initialize(&my12);

	// シーンマネージャー
	sceneManager = SceneManager::GetInstance();
	// fps固定
	frameFixed = FrameFixed::GetInstance();

	// particle関係
	Particle2D::Init(&my12);
	ParticleManager::Init(&my12);
}

void Application::Destroy()
{
	// 解放処理
	input->Destroy();
	Camera::Destroy();
	sceneManager->Destroy();
	xinput->Destroy();
	frameFixed->Destroy();
}

void Application::Update()
{
	// フレームレート固定
	frameFixed->PreWait();

	// 更新
	input->Update();
	xinput->Update();

	ObjectManager::GetInstance()->Update();

	// 各シーンのupdate
	sceneManager->Update();

	Draw();

	// フレームレート固定用
	frameFixed->PostWait();
}

void Application::Draw()
{
	// レンダーテクスチャへの描画
	//postEffect->PreDrawScene();
	//postEffect->PostDrawScene();

	// 描画準備用
	my12.PreDraw();

	//postEffect->Draw();
	sceneManager->Draw();
	// カメラのデバッグ用
	//Camera::DebugDraw();

	// 描画コマンド消化用
	my12.PostDraw();
}
