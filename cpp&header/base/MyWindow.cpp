#include "MyWindow.h"
#include"Setting.h"
#include<Windows.h>
#include<imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

MyWindow::MyWindow()
{

}
MyWindow::~MyWindow()
{
	Release();
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	#ifdef _DEBUG

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return 1;
	}

#endif // _DEBUG


	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウが破壊された場合
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;

	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}


void MyWindow::Initialize(const char* windowName)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = windowName; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル設定

	// ウィンドウクラスをosに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ{ｘ座標　ｙ座標　横幅　縦幅}
	RECT wrc = { 0,0,SETTING_VALUE::WINDOW_WIDTH,SETTING_VALUE::WINDOW_HEIGHT };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName, //　クラス名
		windowName,   // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, //	標準的なウィンドウスタイル
		CW_USEDEFAULT,  // 表示ｘ座標
		CW_USEDEFAULT,  // 表示ｙ座標
		wrc.right - wrc.left,  // ウィンドウ横幅 
		wrc.bottom - wrc.top,  // ウィンドウ縦幅
		nullptr,               // 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		w.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				//	オプション


	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);

}

HWND MyWindow::GetHWND()
{
	return hwnd;
}
void MyWindow::Release()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

WNDCLASSEX MyWindow::GetConfig()
{
	return w;
}