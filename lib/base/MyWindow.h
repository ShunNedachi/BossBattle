#pragma once
#include<Windows.h>
#include<imgui_impl_win32.h>


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

	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
	HWND hwnd;
};
