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

// windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

	#pragma region 初期化処理
	// ウィンドウ用
	MyWindow myw;
	myw.Initialize("Engine");

	Input input;
	input.Initialize(myw.GetConfig().hInstance,myw.GetHWND());

	// DirectX 初期化処理 ここから
	MyDirectX12 my12;
	HRESULT result;

	my12.Debug();
	my12.Initialize(&myw);

	Sprite2D::Init(&my12, &myw);

	// DirectX 初期化処理 ここまで

#pragma endregion



	// 描画初期化


	// スプライト
	Sprite2D titleSprite(0.5f,0.5f);

	Sprite2D::LoadTex(0, L"Resources/texture/title.png");

	titleSprite.CreateSprite(0);
    titleSprite.SetPosition({ 650,600,0 });


	// 　3Dオブジェクト
	Object* obj = new Object();
	obj->Init(&my12, &myw);
	obj->CreateModel("player");

	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };

		// 更新
		input.Update();

	
		obj->Update({ 0,0,0 });
		#pragma region 描画処理

		// 描画準備用
		my12.PreDraw();

		obj->Draw();

		// 描画コマンド消化用
		my12.PostDraw();

		#pragma endregion

	}

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