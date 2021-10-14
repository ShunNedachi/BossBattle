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

public:
	static Camera* GetInstance();
	static void Destroy();


	// �J�����̓���p
	void Update();

	// �J�����̒Ǐ]�pupdate
	void UpdateFollow(XMFLOAT3 target);

	// �J�����̒Ǐ]�p(target�ɒǏ])
	//void FollowTarget();


	// eyeDir�̍X�V
	void UpdateEyeDir();


	// �Q�b�^�[
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	
	// �Z�b�^�[
	static void SetEye(XMFLOAT3 eye) { Camera::eye = eye; }
	static void SetTarget(XMFLOAT3 target) { Camera::target = target;}
	static void SetUp(XMFLOAT3 up) { Camera::up = up; }

protected:
	Camera() {};
	~Camera() {};
	

private:
	static Camera* instance;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

	// �����x�N�g��
	static XMFLOAT3 eyeDir;

	// ���a
	float r = 40;
	// �ܓx �o�x
	float phi;
	float theta;

	// �Ǐ]�p
	static XMFLOAT3 pastTarget;
};

