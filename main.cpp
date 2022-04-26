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

#include<memory>

bool MessageError(MSG msg);

// windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

	#pragma region 初期化処理

	// ウィンドウ用
	MyWindow myw;
	myw.Initialize("Engine");

	// DirectInput,Xinput用初期化
	Input* input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance,myw.GetHWND());
	Xinput* xinput = Xinput::GetInstance();
	Xinput::Initialize();

	// directX12基本初期化
	MyDirectX12 my12;
	my12.Debug();
	my12.Initialize(&myw);

	// オブジェクトマネージャー初期化
	ObjectManager::Initialize(&my12);

	std::unique_ptr<PostEffect> postEffect(new PostEffect());
	PostEffect::Init(&my12);
	postEffect->Initialize();

	// particle2D
	Particle2D::Init(&my12);

	ParticleManager::Init(&my12);

	// シーン管理用
	SceneManager* sceneManager = SceneManager::GetInstance();

	// fps固定用
	FrameFixed* frameFixed = FrameFixed::GetInstance();

#pragma endregion


	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };

		// フレームレート固定
		frameFixed->PreWait();

		#pragma region 更新処理

		// 更新
		input->Update();
		xinput->Update();

		ObjectManager::GetInstance()->Update();

		// exe終了用
		if (xinput->TriggerButton(CONTROLLER_1)& XINPUT_BUTTON_BACK)break;

		// 各シーンのupdate
		sceneManager->Update();

	#pragma endregion

		#pragma region 描画処理

		

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


		#pragma endregion

		// フレームレート固定用
		frameFixed->PostWait();

	}


	// 解放処理
	input->Destroy();
	Camera::Destroy();
	sceneManager->Destroy();
	xinput->Destroy();
	frameFixed->Destroy();

	return 0;
}

bool MessageError(MSG msg)
{
	// メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}
	// 終了メッセージが来たらループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	// 0のとき正常動作
	return false;
}