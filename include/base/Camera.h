#pragma once
#include<DirectXMath.h>
#include"XinputControll.h"
#include"Input.h"
#include"Easing.h"

// �V���O���g���p�^�[��
class Camera
{

private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	Camera() = default;
	~Camera() = default;

	void operator=(const Camera & obj) {}
	Camera(const Camera& obj) {}


public:
	static Camera* GetInstance();
	static void Destroy();


	// �J�����̓���p
	void Update();

	// �J�����̒Ǐ]�pupdate
	void Follow(XMFLOAT3 target);

	// eyeDir�̍X�V
	void UpdateEyeDir();


	// �Q�b�^�[
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static float GetRadius() { return initRadius; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	static XMFLOAT3 GetEndEye() { return endEye; }
	
	static float GetPhi() { return phi; }
	static float GetPhiRadius() { return phi * DirectX::XM_PI / 180; }
	static float GetTheta() { return theta; }
	static float GetThetaRadius() { return theta * DirectX::XM_PI / 180;}

	static XMMATRIX GetViewMatrix() { return matView; }
	static XMMATRIX GetProjection() { return projectionMat; }
	static XMMATRIX GetBillboardMatrix() { return matBillboard; }
	static XMMATRIX GetBillboardYMatrix() { return matBillboradY; }

	// �Z�b�^�[
	static void SetEye(XMFLOAT3 eye) { Camera::eye = eye; }
	static void SetTarget(XMFLOAT3 target) { Camera::target = target;}
	static void SetUp(XMFLOAT3 up) { Camera::up = up; }
	static void SetR(float r) { Camera::initRadius = r; }

	static void SetTheta(float theta) { Camera::theta = theta; }
	static void SetPhi(float phi) { Camera::phi = phi; }

	static void SetState(float r,float theta,float phi){
		Camera::initRadius = r;
		Camera::theta = theta;
		Camera::phi = phi;
	}
	
	// �J�����̃Y�[���p
	void ZoomIn();
	void SetZoomState(float radius, float time,bool endEasing = false);

	void SetEyeLerp();

	// �r���[�s��X�V�p
	void UpdateView();
	// �r���{�[�h�s��p
	void UpdateBillboard();
	// �v���W�F�N�V�����s��p
	void UpdateProjection();

	// �f�o�b�O�p
	static void DebugDraw();

	// �J�������߂Â������p
	void HitObject();

	// �n�ʂƂ̔���p
	bool HitGround();

private:
	static Camera* instance;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

	// �����x�N�g��
	static XMFLOAT3 eyeDir;

	// ���a
	static float initRadius;
	const float settingRadius = 20;
	// �ܓx �o�x
	static float phi;
	static float theta;

	// �Ǐ]�p
	static XMFLOAT3 pastTarget;

	// �r���[�s��
	static XMMATRIX matView;
	// �v���W�F�N�V�����s��
	static XMMATRIX projectionMat;

	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// Y������r���{�[�h�s��
	static XMMATRIX matBillboradY;

	// �J�����̋����p
	static bool zoomFlg;
	float zoomTime;
	float zoomRadius;
	float zoomCount = 0;
	bool zoomEnd = false;

	// ��p
	float fov = 60.0f;

	// �Y�[���������ɃC�[�W���O��������̂��ǂ���
	bool endEasing = false;

	Easing zoomEasing;

	static XMFLOAT3 endEye;
	XMFLOAT3 endTarget;
};

