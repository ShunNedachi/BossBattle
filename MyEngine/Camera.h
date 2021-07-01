#pragma once
#include<DirectXMath.h>

// �V���O���g���p�^�[��
class Camera
{

private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	static Camera* GetInstance();
	static void Destroy();


	// �Q�b�^�[
	XMFLOAT3 GetEye() { return eye; }
	XMFLOAT3 GetTarget() { return target;}
	XMFLOAT3 GetUp() { return up; }
	
	// �Z�b�^�[
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

protected:
	Camera() {};
	~Camera() {};
	

private:
	static Camera* instance;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;
	
};

