#pragma once
#include"Object.h"
#include<DirectXMath.h>

class AttackBase;

class Collision
{
public:

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1Pos">1つ目の球の位置</param>
	/// <param name="sphere2Pos">2つ目の球の位置</param>
	/// <param name="sphereR">1つ目の球の半径</param>
	/// <param name="sphere2R">2つ目の球の半径</param>
	/// <returns></returns>
	static bool Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, 
						  const float& sphereR, const float& sphere2R);

	/// <summary>
	/// objと攻撃の当たり判定
	/// </summary>
	/// <param name="obj">判定したいオブジェクト</param>
	/// <param name="attack">判定したい攻撃</param>
	/// <returns></returns>
	static bool Attack2OBJ(Object* obj, AttackBase* attack);
};

