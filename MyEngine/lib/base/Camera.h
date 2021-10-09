#pragma once
#include<DirectXMath.h>
#include"XinputControll.h"
#include"Input.h"

// シングルトンパターン
class Camera
{

private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static Camera* GetInstance();
	static void Destroy();


	// カメラの動作用
	void Update();

	// カメラの回転用
	void UpdateRot();

	// ゲッター
	XMFLOAT3 GetEye() { return eye; }
	XMFLOAT3 GetTarget() { return target;}
	XMFLOAT3 GetUp() { return up; }
	
	// セッター
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
	
	static Input* input;
	static Xinput xinput;
};

