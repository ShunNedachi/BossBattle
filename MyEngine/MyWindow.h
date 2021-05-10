#pragma once
#include<Windows.h>

class MyWindow
{
public:
	MyWindow();
	~MyWindow();

	void Initialize(const char* windowName);

	HWND GetHWND();
	void Release();
	const int GetWidth();
	const int GetHeight();
	WNDCLASSEX GetConfig();
private:

	const int window_width = 1280; // 横幅
	const int window_height = 720; // 縦幅

	WNDCLASSEX w{}; // ウィンドウクラスの設定
	HWND hwnd;
};
