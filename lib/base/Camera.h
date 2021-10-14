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

	// カメラの追従用update
	void UpdateFollow(XMFLOAT3 target);

	// カメラの追従用(targetに追従)
	//void FollowTarget();


	// eyeDirの更新
	void UpdateEyeDir();


	// ゲッター
	static XMFLOAT3 GetEye() { return eye; }
	static XMFLOAT3 GetTarget() { return target;}
	static XMFLOAT3 GetUp() { return up; }
	static XMFLOAT3 GetEyeDir() { return eyeDir; }
	
	// セッター
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

	// 視線ベクトル
	static XMFLOAT3 eyeDir;

	// 半径
	float r = 40;
	// 緯度 経度
	float phi;
	float theta;

	// 追従用
	static XMFLOAT3 pastTarget;
};

