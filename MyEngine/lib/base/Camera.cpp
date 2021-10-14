#include "Camera.h"

// �ÓI�����o�ϐ�
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::pastTarget = { 0,0,0 };
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


	// �����x�N�g���̍X�V
	UpdateEyeDir();
}

void Camera::UpdateFollow(XMFLOAT3 target)
{
	using namespace DirectX;
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// �萔��`
	const int MIN_PHI_RADIUS = 0;
	const int MAX_PHI_RADIUS = 360;
	const int MIN_THETA_RADIUS = 0;
	const int MAX_THETA_RADIUS = 89;

	if (input->PushKey(DIK_UP)|| xinput->MoveStick(0,xinput_RS)&XINPUT_STICK_UP)theta++;
	else if (input->PushKey(DIK_DOWN)|| xinput->MoveStick(0,xinput_RS)&XINPUT_STICK_DOWN)theta--;

	if (input->PushKey(DIK_RIGHT)||xinput->MoveStick(0,xinput_RS)&XINPUT_STICK_RIGHT)phi++;
	else if (input->PushKey(DIK_LEFT)|| xinput->MoveStick(0,xinput_RS)&XINPUT_STICK_LEFT)phi--;

	// �ő�l�ŏ��l�̐ݒ�
	if (phi > MAX_PHI_RADIUS)phi = MIN_PHI_RADIUS;
	else if (phi <= MIN_PHI_RADIUS)phi = MAX_PHI_RADIUS;

	if (theta > MAX_THETA_RADIUS)theta = MAX_THETA_RADIUS;
	else if (theta < MIN_THETA_RADIUS)theta = MIN_THETA_RADIUS;


	// �x�����W�A���ɕύX
	float phiRADIUS = phi * XM_PI / 180;
	float thetaRADIUS = theta * XM_PI / 180;

	float eyeX = cos(phiRADIUS) * cos(thetaRADIUS) * r + target.x;
	float eyeY = sin(thetaRADIUS) * r + target.y;
	float eyeZ = sin(phiRADIUS) * cos(thetaRADIUS) * r + target.z;



	SetEye({ eyeX,eyeY,eyeZ });
	SetTarget(target);

	// �����x�N�g���̍X�V
	UpdateEyeDir();

	//
	pastTarget = target;
}


void Camera::UpdateEyeDir()
{
	XMVECTOR eyeDirV = { target.x - eye.x,target.y - eye.y,target.z - eye.z,0 };
	eyeDirV = DirectX::XMVector3Normalize(eyeDirV);
	DirectX::XMStoreFloat3(&eyeDir, eyeDirV);
}