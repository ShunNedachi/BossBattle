#include "Camera.h"

// Ã“Iƒƒ“ƒo•Ï”
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::up = { 0,1,0 };
Input* Camera::input = Input::GetInstance();
Xinput Camera::xinput;


Camera* Camera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera;
	}

	return instance;
}

void Camera::Destroy()
{
	delete instance;

	instance = nullptr;
}

void Camera::Update()
{
	xinput.Update();
	
	if (input->PushKey(DIK_W) || xinput.MoveStick(0, xinput_LS) & XINPUT_STICK_UP)
	{
		if (eye.z < -1)eye.z++;
	}
	else if (input->PushKey(DIK_S) || xinput.MoveStick(0, xinput_LS) & XINPUT_STICK_DOWN)
	{
		eye.z--;
	}

	SetEye(eye);
}