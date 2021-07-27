#include "Camera.h"

// Ã“Iƒƒ“ƒo•Ï”
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::up = { 0,1,0 };

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