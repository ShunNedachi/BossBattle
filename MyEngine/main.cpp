#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include"ObjectManager.h"


bool MessageError(MSG msg);

// windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

#pragma region 初期化処理
	// ウィンドウ用
	MyWindow myw;
	myw.Initialize("Engine");

	Input* input = Input::GetInstance();
	input->Initialize(myw.GetConfig().hInstance,myw.GetHWND());

	Xinput xinput;

	// DirectX 初期化処理 ここから
	MyDirectX12 my12;

	my12.Debug();
	my12.Initialize(&myw);



	// DirectX 初期化処理 ここまで

#pragma endregion

	Camera* camera = Camera::GetInstance();

	SceneManager* sceneManager = SceneManager::GetInstance();


	// 描画初期化
	ObjectManager* objManager;
	objManager = ObjectManager::GetInstance();
	objManager->Initialize(&my12, &myw, camera);

	// 　3Dオブジェクト
	objManager->AddOBJ("bless");
	objManager->AddOBJ("bless", { 0,0,-50 });

	// スプライト
	objManager->AddSprite(0);

	int objNum = 2;

	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };

		// 更新
		input->Update();
		xinput.Update();


		if (input->TriggerKey(DIK_1))
		{
			sceneManager->NextScene();
		}

		if (input->TriggerKey(DIK_2))
		{
			objNum--;
			objManager->DeleteOBJ(0);
		}

		if (input->TriggerKey(DIK_3))
		{
			objNum++;
			objManager->AddOBJ("bless", {0,0,-50 * (float)objNum});
		}

		// 各シーンのupdate
		// 現在は使用していないが、後々子シーンクラスに処理を移行
		sceneManager->Update();

		objManager->Update();
	
		camera->Update();

		#pragma region 描画処理

		// 描画準備用
		my12.PreDraw();


		objManager->Draw();

		// 描画コマンド消化用
		my12.PostDraw();

		#pragma endregion

	}


	// 解放処理
	input->Destroy();
	camera->Destroy();
	sceneManager->Destroy();
	objManager->Destroy();

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