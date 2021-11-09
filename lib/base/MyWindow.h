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

	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
	HWND hwnd;
};
