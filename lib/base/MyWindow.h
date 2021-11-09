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
	WNDCLASSEX GetConfig();
private:

	WNDCLASSEX w{}; // ウィンドウクラスの設定
	HWND hwnd;
};
