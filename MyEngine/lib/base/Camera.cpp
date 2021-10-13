#include "Camera.h"

// 静的メンバ変数
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::up = { 0,1,0 };
DirectX::XMFLOAT3 Camera::eyeDir = { 0,0,0 };



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
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	if (input->PushKey(DIK_W) || xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_UP)
	{
		eye.y++;
	}
	else if (input->PushKey(DIK_S) || xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_DOWN)
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


	XMVECTOR eyeDirV = { target.x - eye.x,target.y - eye.y,target.z - eye.z,0 };
	eyeDirV = DirectX::XMVector3Normalize(eyeDirV);
	DirectX::XMStoreFloat3(&eyeDir, eyeDirV);

}

void Camera::UpdateRot()
{
	using namespace DirectX;
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// 定数定義
	const int MIN_PHI_RADIUS = 0;
	const int MAX_PHI_RADIUS = 360;
	const int MIN_THETA_RADIUS = 0;
	const int MAX_THETA_RADIUS = 89;

	if (input->PushKey(DIK_UP))theta++;
	else if (input->PushKey(DIK_DOWN))theta--;

	if (input->PushKey(DIK_RIGHT))phi++;
	else if (input->PushKey(DIK_LEFT))phi--;

	// 最大値最小値の設定
	if (phi > MAX_PHI_RADIUS)phi = MIN_PHI_RADIUS;
	else if (phi <= MIN_PHI_RADIUS)phi = MAX_PHI_RADIUS;

	if (theta > MAX_THETA_RADIUS)theta = MAX_THETA_RADIUS;
	else if (theta < MIN_THETA_RADIUS)theta = MIN_THETA_RADIUS;


	// 度をラジアンに変更
	float phiRADIUS = phi * XM_PI / 180;
	float thetaRADIUS = theta * XM_PI / 180;

	float eyeX = cos(phiRADIUS) * cos(thetaRADIUS) * r;
	float eyeY = sin(thetaRADIUS) * r;
	float eyeZ = sin(phiRADIUS) * cos(thetaRADIUS) * r;

	SetEye({ eyeX,eyeY,eyeZ });

}

void Camera::FollowTarget()
{
	XMFLOAT3 followEye = { target.x + 0,target.y + 10, target.z + 40 };
	XMFLOAT3 followUp = { 0,10,0 };
	//SetEye(followEye);
	//SetUp(followUp);
}