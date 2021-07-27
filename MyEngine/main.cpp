#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Object.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"


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

	Sprite2D::Init(&my12, &myw);

	Camera* camera = Camera::GetInstance();

	SceneManager* sceneManager = SceneManager::GetInstance();

	// DirectX 初期化処理 ここまで

#pragma endregion



	// 描画初期化


	// スプライト
	Sprite2D titleSprite(0.5f,0.5f);

	Sprite2D::LoadTex(0, L"Resources/texture/title.png");

	titleSprite.CreateSprite(0);
    titleSprite.SetPosition({ 650,600,0 });


	// 　3Dオブジェクト
	Object* obj = new Object(NORMAL);
	obj->Init(&my12, &myw,camera);
	obj->CreateModel("bless");

	Object* obj2 = new Object(TOON);
	obj2->CreateModel("bless");
	

	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };

		// 更新
		input->Update();
		xinput.Update();

		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 target = camera->GetTarget();

		if (input->PushKey(DIK_W) || xinput.MoveStick(0,xinput_LS) & XINPUT_STICK_UP)
		{
			 if(eye.z < -1)eye.z++;
		}
		else if (input->PushKey(DIK_S)|| xinput.MoveStick(0,xinput_LS)& XINPUT_STICK_DOWN )
		{
			eye.z--;
		}

		if (input->TriggerKey(DIK_1))
		{
			sceneManager->NextScene();
		}
	
		camera->SetEye(eye);


		obj->Update({ 0,0,0 });
		obj2->Update({ 0,1,0 });
		#pragma region 描画処理

		// 描画準備用
		my12.PreDraw();

		//obj->Draw(NORMAL);
		obj->Draw();

		// 描画コマンド消化用
		my12.PostDraw();

		#pragma endregion

	}

	delete obj;
	delete obj2;

	input->Destroy();
	camera->Destroy();
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