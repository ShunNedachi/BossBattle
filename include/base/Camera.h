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
	using XMMATRIX = DirectX::XMMATRIX;

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
	static XMFLOAT3 GetEndEye() { return endEye; }
	
	static float GetPhi() { return phi; }
	static float GetPhiRadius() { return phi * DirectX::XM_PI / 180; }
	static float GetTheta() { return theta; }
	static float GetThetaRadius() { return theta * DirectX::XM_PI / 180;}

	static XMMATRIX GetViewMatrix() { return matView; }
	static XMMATRIX GetProjection() { return projectionMat; }
	static XMMATRIX GetBillboardMatrix() { return matBillboard; }
	static XMMATRIX GetBillboardYMatrix() { return matBillboradY; }

	// セッター
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
	
	// カメラのズーム用
	void ZoomIn();
	void SetZoomState(float radius, float time,bool endEasing = false);

	void SetEyeLerp();

	// ビュー行列更新用
	void UpdateView();
	// ビルボード行列用
	void UpdateBillboard();
	// プロジェクション行列用
	void UpdateProjection();

	// デバッグ用
	static void DebugDraw();

	// カメラが近づく処理用
	void HitObject();

	// 地面との判定用
	bool HitGround();

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

	// ビュー行列
	static XMMATRIX matView;
	// プロジェクション行列
	static XMMATRIX projectionMat;

	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸周りビルボード行列
	static XMMATRIX matBillboradY;

	// カメラの挙動用
	static bool zoomFlg;
	float zoomTime;
	float zoomRadius;
	float zoomCount = 0;
	bool zoomEnd = false;

	// 画角
	float fov = 60.0f;

	// ズーム解除時にイージングをかけるのかどうか
	bool endEasing = false;

	Easing zoomEasing;

	static XMFLOAT3 endEye;
	XMFLOAT3 endTarget;
};

