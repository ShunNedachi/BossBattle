#pragma once
#include<DirectXMath.h>
#include"XinputControll.h"
#include"Input.h"
#include"Easing.h"

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
	void Follow(XMFLOAT3 target);

	// eyeDirの更新
	void UpdateEyeDir();


	// ゲッター
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static float GetRadius() { return initRadius; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	
	static float GetPhi() { return phi; }
	static float GetPhiRadius() { return phi * DirectX::XM_PI / 180; }
	static float GetTheta() { return theta; }
	static float GetThetaRadius() { return theta * DirectX::XM_PI / 180;}


	// セッター
	static void SetEye(XMFLOAT3 eye) { Camera::eye = eye; }
	static void SetTarget(XMFLOAT3 target) { Camera::target = target;}
	static void SetUp(XMFLOAT3 up) { Camera::up = up; }
	static void SetR(float r) { Camera::initRadius = r; }

	static void SetTheta(float theta) { Camera::theta = theta; }
	static void SetPhi(float phi) { Camera::phi = phi; }


	
	// カメラのズーム用
	void ZoomIn();
	void SetZoomState(float radius, float time,bool endEasing = false);

	void SetEyeLerp();

private:
	static Camera* instance;

	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static XMFLOAT3 up;

	// 視線ベクトル
	static XMFLOAT3 eyeDir;

	// 半径
	static float initRadius;
	const float settingRadius = 20;
	// 緯度 経度
	static float phi;
	static float theta;

	// 追従用
	static XMFLOAT3 pastTarget;

	// カメラの挙動用
	static bool zoomFlg;
	float zoomTime;
	float zoomRadius;
	float zoomCount = 0;
	bool zoomEnd = false;

	// ズーム解除時にイージングをかけるのかどうか
	bool endEasing = false;

	Easing zoomEasing;

	XMFLOAT3 endEye;
	XMFLOAT3 endTarget;
};

