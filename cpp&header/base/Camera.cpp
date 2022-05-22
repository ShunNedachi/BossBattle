#include "Camera.h"
#include"Setting.h"
#include"GameFunction.h"
#include<imgui.h>
#include"Collision.h"

// �ÓI�����o�ϐ�
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
	// �v���C���[�̕K�E�Z��
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

	// �����x�N�g���̍X�V
	UpdateEyeDir();

	// �r���{�[�h�s��̍X�V
	UpdateBillboard();
	// �v���W�F�N�V�����s��̍X�V
	UpdateProjection();
}

void Camera::Follow(XMFLOAT3 target)
{
	using namespace DirectX;
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// �萔��`
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

	// �ő�l�ŏ��l�̐ݒ�
	if (phi > MAX_PHI_RADIUS)phi = MIN_PHI_RADIUS;
	else if (phi <= MIN_PHI_RADIUS)phi = MAX_PHI_RADIUS;

	if (theta > MAX_THETA_RADIUS)theta = MAX_THETA_RADIUS;
	else if (theta < MIN_THETA_RADIUS)theta = MIN_THETA_RADIUS;


	// �x�����W�A���ɕύX
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
	// ���_�A�����_�A�����
	XMVECTOR eyePos = DirectX::XMLoadFloat3(&eye);
	XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
	XMVECTOR upVector = DirectX::XMLoadFloat3(&up);

	// �J����z��
	XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPos, eyePos);
	// �x�N�g���𐳋K��
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);
	// �J������x��
	XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	// ���K��
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);
	// �J������y��
	XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�����[���h���W�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��(�t��])���v�Z
	matView = DirectX::XMMatrixTranspose(matCameraRot);

	// ���_���W��-1�����������W
	XMVECTOR eyePosInverse = DirectX::XMVectorNegate(eyePos);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, eyePosInverse);
	XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, eyePosInverse);
	XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, eyePosInverse);
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR traslation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = traslation;

}

void Camera::UpdateBillboard()
{
	// ���_�A�����_�A�����
	XMVECTOR eyePos = DirectX::XMLoadFloat3(&eye);
	XMVECTOR targetPos = DirectX::XMLoadFloat3(&target);
	XMVECTOR upVector = DirectX::XMLoadFloat3(&up);

	// �J����z��
	XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(targetPos, eyePos);
	// �x�N�g���𐳋K��
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);
	// �J������x��
	XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);
	// ���K��
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);
	// �J������y��
	XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ,cameraAxisX);

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�����[���h���W�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��(�t��])���v�Z
	matView = DirectX::XMMatrixTranspose(matCameraRot);

	// ���_���W��-1�����������W
	XMVECTOR eyePosInverse = DirectX::XMVectorNegate(eyePos);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = DirectX::XMVector3Dot(cameraAxisX, eyePosInverse);
	XMVECTOR tY = DirectX::XMVector3Dot(cameraAxisY, eyePosInverse);
	XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxisZ, eyePosInverse);
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR traslation = DirectX::XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = traslation;


	// �S�����r���{�[�h�̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	// Y���r���{�[�h�s��̌v�Z
	
	// �J����X���AY���AZ��
	XMVECTOR yBillCameraAxisX, yBillCameraAxisY, yBillCameraAxisZ;

	// X���͋���
	yBillCameraAxisX = cameraAxisX; 
	// Y���̓��[���h���W�n��Y��
	yBillCameraAxisY = DirectX::XMVector3Normalize(upVector);
	// Z����X����Y���̊O�ςŋ��߂�
	yBillCameraAxisZ = DirectX::XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);

	// Y���r���{�[�h�s��
	matBillboradY.r[0] = yBillCameraAxisX;
	matBillboradY.r[1] = yBillCameraAxisY;
	matBillboradY.r[2] = yBillCameraAxisZ;
	matBillboradY.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

}

void Camera::UpdateProjection()
{
	// �ˉe�ϊ�
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

	// �b���J�E���g���@������
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
