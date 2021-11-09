#pragma once
#include<DirectXMath.h>
#include"XinputControll.h"
#include"Input.h"

// �V���O���g���p�^�[��
class Camera
{

private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

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
	void UpdateFollow(XMFLOAT3 target);

	// eyeDir�̍X�V
	void UpdateEyeDir();


	// �Q�b�^�[
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	
	static float GetPhi() { return phi; }
	static float GetPhiRadius() { return phi * DirectX::XM_PI / 180; }
	static float GetTheta() { return theta; }
	static float GetThetaRadius() { return theta * DirectX::XM_PI / 180;}
	static float GetRadius() { return r; }


	// �Z�b�^�[
	static void SetEye(XMFLOAT3 eye) { Camera::eye = eye; }
	static void SetTarget(XMFLOAT3 target) { Camera::target = target;}
	static void SetUp(XMFLOAT3 up) { Camera::up = up; }

	static void SetTheta(float theta) { Camera::theta = theta; }
	static void SetPhi(float phi) { Camera::phi = phi; }

private:
	static Camera* instance;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

	// �����x�N�g��
	static XMFLOAT3 eyeDir;

	// ���a
	static float r;
	// �ܓx �o�x
	static float phi;
	static float theta;

	// �Ǐ]�p
	static XMFLOAT3 pastTarget;
};

