#include "Camera.h"
#include"Setting.h"
#include"GameFunction.h"
#include<imgui.h>
#include"Collision.h"

// 静的メンバ変数
Camera* Camera::instance = nullptr;
DirectX::XMFLOAT3 Camera::eye = { 0,0,-100 };
DirectX::XMFLOAT3 Camera::endEye = { 0,0,0 };
DirectX::XMFLOAT3 Camera::target = { 0,0,0 };
DirectX::XMFLOAT3 Camera::pastTarget = { 0,0,0 };
DirectX::XMFLOAT3 Camera::up = { 0,1,0 };
DirectX::XMFLOAT3 Camera::eyeDir = { 0,0,0 };
float Camera::phi = 0;
float Camera::theta = 0;
float Camera::initRadius = 20;
bool Camera::zoomFlg = false;
DirectX::XMMATRIX Camera::matView = {};
DirectX::XMMATRIX Camera::projectionMat = {};
DirectX::XMMATRIX Camera::matBillboard = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Camera::matBillboradY = DirectX::XMMatrixIdentity();


Camera* Camera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
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
	if (zoomFlg && zoomEnd)
	{
		ZoomIn();
	}
	else
	{
		initRadius = 20;
	}

	if (GameFunction::GetPlayerIsSpecial())
	{
		initRadius = 5;
	}
	else
	{
		initRadius = 20;
	}


	SetEyeLerp();
	//SetEye(endEye);
	SetTarget(endTarget);
	//SetTargetLerp();

	// 視線ベクトルの更新
	UpdateEyeDir();

	// ビルボード行列の更新
	UpdateBillboard();
	// プロジェクション行列の更新
	UpdateProjection();
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
	const int MAX_THETA_RADIUS = 70;

	if (input->PushKey(DIK_DOWN) || xinput->MoveStick(0, XINPUT_BUTTON_RS) & XINPUT_STICK_DOWN)
	{
		theta += SETTING_VALUE::CAMERA_MOVE_VALUE;
	}
	else if (input->PushKey(DIK_UP) || xinput->MoveStick(0, XINPUT_BUTTON_RS) & XINPUT_STICK_UP)
	{
		theta -= SETTING_VALUE::CAMERA_MOVE_VALUE;
	}

	if (input->PushKey(DIK_RIGHT) || xinput->MoveStick(0, XINPUT_BUTTON_RS) & XINPUT_STICK_RIGHT)
	{
		phi -= SETTING_VALUE::CAMERA_MOVE_VALUE;
	}
	else if (input->PushKey(DIK_LEFT) || xinput->MoveStick(0, XINPUT_BUTTON_RS) & XINPUT_STICK_LEFT)
	{
		phi += SETTING_VALUE::CAMERA_MOVE_VALUE;
	}

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
	
	result = DirectX::XMVectorLerp(eyeV, endEyeV,0.1f);

	DirectX::XMStoreFloat3(&eye, result);

	SetEye(eye);
}

void Camera::UpdateView()
{
	// 視点、注視点、上方向
	XMVECTOR eyePos = DirectX::XMLoadFloat3(&eye);
	XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
	XMVECTOR upVector = DirectX::XMLoadFloat3(&up);

	// カメラz軸
	XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPos, eyePos);
	// ベクトルを正規化
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);
	// カメラのx軸
	XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	// 正規化
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);
	// カメラのy軸
	XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標→ワールド座標の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列(逆回転)を計算
	matView = DirectX::XMMatrixTranspose(matCameraRot);

	// 視点座標に-1をかけた座標
	XMVECTOR eyePosInverse = DirectX::XMVectorNegate(eyePos);
	// カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, eyePosInverse);
	XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, eyePosInverse);
	XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, eyePosInverse);
	// 一つのベクトルにまとめる
	XMVECTOR traslation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = traslation;

}

void Camera::UpdateBillboard()
{
	// 視点、注視点、上方向
	XMVECTOR eyePos = DirectX::XMLoadFloat3(&eye);
	XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
	XMVECTOR upVector = DirectX::XMLoadFloat3(&up);

	// カメラz軸
	XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPos, eyePos);
	// ベクトルを正規化
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);
	// カメラのx軸
	XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	// 正規化
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);
	// カメラのy軸
	XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ,cameraAxisX);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標→ワールド座標の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列(逆回転)を計算
	matView = DirectX::XMMatrixTranspose(matCameraRot);

	// 視点座標に-1をかけた座標
	XMVECTOR eyePosInverse = DirectX::XMVectorNegate(eyePos);
	// カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, eyePosInverse);
	XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, eyePosInverse);
	XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, eyePosInverse);
	// 一つのベクトルにまとめる
	XMVECTOR traslation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = traslation;


	// 全方向ビルボードの計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	// Y軸ビルボード行列の計算
	
	// カメラX軸、Y軸、Z軸
	XMVECTOR yBillCameraAxisX, yBillCameraAxisY, yBillCameraAxisZ;

	// X軸は共通
	yBillCameraAxisX = cameraAxisX; 
	// Y軸はワールド座標系のY軸
	yBillCameraAxisY = DirectX::XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求める
	yBillCameraAxisZ = DirectX::XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);

	// Y軸ビルボード行列
	matBillboradY.r[0] = yBillCameraAxisX;
	matBillboradY.r[1] = yBillCameraAxisY;
	matBillboradY.r[2] = yBillCameraAxisZ;
	matBillboradY.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

}

void Camera::UpdateProjection()
{
	// 射影変換
	projectionMat = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(fov),
		(float)SETTING_VALUE::WINDOW_WIDTH / SETTING_VALUE::WINDOW_HEIGHT,
		0.1f, 1000.0f
	);
}

void Camera::ZoomIn()
{
	zoomCount++;

	initRadius = zoomEasing.StartEeaging(easeInFLAG);

	// 秒数カウント分　続ける
	if (zoomCount >= (zoomTime * SETTING_VALUE::GAME_FRAME))
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


void Camera::DebugDraw()
{
	#ifdef _DEBUG
	
	ImGui::Begin("Camera");
	ImGui::Text("EYE position : %f %f %f", eye.x, eye.y, eye.z);
	ImGui::Text("EYE endPosition : %f %f %f", endEye.x, endEye.y, endEye.z);
	ImGui::End();

#endif // _DEBUG

}

void Camera::HitObject()
{
}

bool Camera::HitGround()
{
	//Collision::CheckRay2Plane()
	return false;
}

void Camera::Rotation()
{

}
