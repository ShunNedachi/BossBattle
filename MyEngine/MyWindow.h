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

	const int window_width = 1280; // ����
	const int window_height = 720; // �c��

	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
	HWND hwnd;
};
