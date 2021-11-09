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

private:
	Camera() = default;
	~Camera() = default;

	void operator=(const Camera & obj) {}
	Camera(const Camera& obj) {}


public:
	static Camera* GetInstance();
	static void Destroy();


	// カメラの動作用
	void Update();

	// カメラの追従用update
	void UpdateFollow(XMFLOAT3 target);

	// eyeDirの更新
	void UpdateEyeDir();


	// ゲッター
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	
	static float GetPhi() { return phi; }
	static float GetPhiRadius() { return phi * DirectX::XM_PI / 180; }
	static float GetTheta() { return theta; }
	static float GetThetaRadius() { return theta * DirectX::XM_PI / 180;}
	static float GetRadius() { return r; }


	// セッター
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

	// 視線ベクトル
	static XMFLOAT3 eyeDir;

	// 半径
	static float r;
	// 緯度 経度
	static float phi;
	static float theta;

	// 追従用
	static XMFLOAT3 pastTarget;
};

