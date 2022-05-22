#include"Application.h"
#include"Input.h"
#include "XinputControll.h"

bool MessageError(MSG msg);


// windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::GetInstance()->Initialize();


	MSG msg{}; // メッセージ
	while (true)
	{
		if (MessageError(msg)) { break; };


		// exe終了用
		if (Xinput::GetInstance()->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_BACK)break;

		Application::GetInstance()->Update();

	}
	
	Application::GetInstance()->Destroy();

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

