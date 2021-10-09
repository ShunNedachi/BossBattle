#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include<thread>


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
	Xinput xinput;


	// directX12基本初期化
	MyDirectX12 my12;
	my12.Debug();
	my12.Initialize(&myw);

	// オブジェクトマネージャー初期化
	ObjectManager::Initialize(&my12, &myw, Camera::GetInstance());

	// シーン管理用
	SceneManager* sceneManager = SceneManager::GetInstance();


#pragma endregion


	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };

		// 更新
		input->Update();
		xinput.Update();


		// 各シーンのupdate
		sceneManager->Update();


		#pragma region 描画処理

		// 描画準備用
		my12.PreDraw();

		sceneManager->Draw();

		// 描画コマンド消化用
		my12.PostDraw();

		#pragma endregion

	}


	// 解放処理
	input->Destroy();
	Camera::Destroy();
	sceneManager->Destroy();

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