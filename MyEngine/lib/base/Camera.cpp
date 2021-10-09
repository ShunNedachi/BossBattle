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
		eye.y++;
	}
	else if (input->PushKey(DIK_S) || xinput.MoveStick(0, xinput_LS) & XINPUT_STICK_DOWN)
	{
		eye.y--;
	}

	if (input->PushKey(DIK_A))
	{
		eye.x++;
	}
	else if (input->PushKey(DIK_D))
	{
		eye.x--;
	}

	SetEye(eye);
}

void Camera::UpdateRot()
{
	using namespace DirectX;
	xinput.Update();

	XMVECTOR axis;

	// ã‰º•ûŒü‚ÌƒJƒƒ‰‚Ì‰ñ“]
	if (input->PushKey(DIK_UP))
	{	
		// xŽ²‰ñ“]
		axis = { 1,0,0,0 };
		XMVECTOR qt = XMQuaternionRotationAxis(axis, XMConvertToRadians(1));
		XMVECTOR eyeV = { eye.x,eye.y,eye.z,0 };
		XMVECTOR qt_Inverce = XMQuaternionRotationAxis(axis, XMConvertToRadians(-1));

		XMVECTOR result = XMQuaternionMultiply(eyeV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&eye, result);


		SetEye(eye);

		// up‚É‚à“¯‚¶‚±‚Æ‚ð‚·‚é
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		result = XMQuaternionMultiply(upV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&up, result);

		SetUp(up);
	}
	else if (input->PushKey(DIK_DOWN))
	{
		axis = { -1,0,0,0 };
		XMVECTOR qt = XMQuaternionRotationAxis(axis, XMConvertToRadians(1));
		XMVECTOR eyeV = { eye.x,eye.y,eye.z,0 };
		XMVECTOR qt_Inverce = XMQuaternionRotationAxis(axis, XMConvertToRadians(-1));

		XMVECTOR result = XMQuaternionMultiply(eyeV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&eye, result);

		SetEye(eye);
				// up‚É‚à“¯‚¶‚±‚Æ‚ð‚·‚é
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		result = XMQuaternionMultiply(upV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&up, result);

		SetUp(up);

	}

	// ‰¡•ûŒü‚ÌƒJƒƒ‰‚Ì‰ñ“]
	if (input->PushKey(DIK_RIGHT))
	{
		axis = { 0,-1,0,0 };
		XMVECTOR qt = XMQuaternionRotationAxis(axis, XMConvertToRadians(1));
		XMVECTOR eyeV = { eye.x,eye.y,eye.z,0 };
		XMVECTOR qt_Inverce = XMQuaternionRotationAxis(axis, XMConvertToRadians(-1));

		XMVECTOR result = XMQuaternionMultiply(eyeV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&eye, result);

		SetEye(eye);

		// up‚É‚à“¯‚¶‚±‚Æ‚ð‚·‚é
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		result = XMQuaternionMultiply(upV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&up, result);

		SetUp(up);

	}
	else if (input->PushKey(DIK_LEFT))
	{
		axis = { 0,1,0,0 };
		XMVECTOR qt = XMQuaternionRotationAxis(axis, XMConvertToRadians(1));
		XMVECTOR eyeV = { eye.x,eye.y,eye.z,0 };
		XMVECTOR qt_Inverce = XMQuaternionRotationAxis(axis, XMConvertToRadians(-1));

		XMVECTOR result = XMQuaternionMultiply(eyeV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&eye, result);

		SetEye(eye);

		// up‚É‚à“¯‚¶‚±‚Æ‚ð‚·‚é
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		result = XMQuaternionMultiply(upV, qt);
		result = XMQuaternionMultiply(qt_Inverce, result);

		XMStoreFloat3(&up, result);

		SetUp(up);

	}
}