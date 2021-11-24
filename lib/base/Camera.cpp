#include "Camera.h"
#include"Setting.h"
#include"GameFunction.h"

// 静的メンバ変数
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::pastTarget = { 0,0,0 };
DirectX::XMFLOAT3 Camera::up = { 0,1,0 };
DirectX::XMFLOAT3 Camera::eyeDir = { 0,0,0 };
float Camera::phi = 0;
float Camera::theta = 0;
float Camera::initRadius = 20;
bool Camera::zoomFlg = false;


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
	// プレイヤーの必殺技時
	//if (zoomFlg && zoomEnd)
	//{
	//	ZoomIn();
	//}
	//else
	//{
	//	initRadius = 20;
	//}

	if (GameFunction::GetPlayerIsSpecial())
	{
		initRadius = 5;
	}
	else
	{
		initRadius = 20;
	}


	SetEyeLerp();
	SetTarget(endTarget);
	//SetEye(endEye);
	//SetTargetLerp();

	// 視線ベクトルの更新
	UpdateEyeDir();
}

void Camera::Follow(XMFLOAT3 target)
{
	using namespace DirectX;
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// 定数定義
	const int MIN_PHI_RADIUS = 0;
	const int MAX_PHI_RADIUS = 360;
	const int MIN_THETA_RADIUS = 0;
	const int MAX_THETA_RADIUS = 89;

	if (input->PushKey(DIK_DOWN)|| xinput->MoveStick(0,XINPUT_BUTTON_RS)&XINPUT_STICK_DOWN)theta+= CAMERA_MOVE_VALUE;
	else if (input->PushKey(DIK_UP)|| xinput->MoveStick(0,XINPUT_BUTTON_RS)&XINPUT_STICK_UP)theta-= CAMERA_MOVE_VALUE;

	if (input->PushKey(DIK_RIGHT)||xinput->MoveStick(0,XINPUT_BUTTON_RS)&XINPUT_STICK_RIGHT)phi-= CAMERA_MOVE_VALUE;
	else if (input->PushKey(DIK_LEFT)|| xinput->MoveStick(0,XINPUT_BUTTON_RS)&XINPUT_STICK_LEFT)phi+= CAMERA_MOVE_VALUE;

	// 最大値最小値の設定
	if (phi > MAX_PHI_RADIUS)phi = MIN_PHI_RADIUS;
	else if (phi <= MIN_PHI_RADIUS)phi = MAX_PHI_RADIUS;

	if (theta > MAX_THETA_RADIUS)theta = MAX_THETA_RADIUS;
	else if (theta < MIN_THETA_RADIUS)theta = MIN_THETA_RADIUS;


	// 度をラジアンに変更
	float phiRADIUS = phi * XM_PI / 180;
	float thetaRADIUS = theta * XM_PI / 180;

	float eyeX = cos(phiRADIUS) * cos(thetaRADIUS) * initRadius + target.x;
	float eyeY = sin(thetaRADIUS) * initRadius + target.y;
	float eyeZ = sin(phiRADIUS) * cos(thetaRADIUS) * initRadius + target.z;

	endEye = { eyeX,eyeY,eyeZ };


	//
	endTarget = target;
}


void Camera::UpdateEyeDir()
{
	XMVECTOR eyeDirV = { target.x - eye.x,target.y - eye.y,target.z - eye.z,0 };
	eyeDirV = DirectX::XMVector3Normalize(eyeDirV);
	DirectX::XMStoreFloat3(&eyeDir, eyeDirV);
}

void Camera::SetEyeLerp()
{
	XMVECTOR result;
	XMVECTOR eyeV = { eye.x,eye.y,eye.z,0 };
	XMVECTOR endEyeV = { endEye.x,endEye.y,endEye.z,0 };
	
	result = DirectX::XMVectorLerp(eyeV, endEyeV,0.08f);

	DirectX::XMStoreFloat3(&eye, result);

	SetEye(eye);
}

void Camera::ZoomIn()
{
	zoomCount++;

	initRadius = zoomEasing.StartEeaging(easeInFLAG);

	// 秒数カウント分　続ける
	if (zoomCount >= (zoomTime * GAME_FRAME))
	{
		zoomFlg = false;
		zoomCount = 0;
		zoomEnd = true;
	}

}

void Camera::SetZoomState(float radius, float time,bool endEasing)
{
	zoomRadius = radius;
	zoomTime = time;
	zoomFlg = true;

	zoomEasing.SetState(time, initRadius, radius);
}
