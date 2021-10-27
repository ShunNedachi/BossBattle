#pragma once
#include"Object.h"
#include<DirectXMath.h>

class AttackBase;

class Collision
{
public:

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere1Pos">1�ڂ̋��̈ʒu</param>
	/// <param name="sphere2Pos">2�ڂ̋��̈ʒu</param>
	/// <param name="sphereR">1�ڂ̋��̔��a</param>
	/// <param name="sphere2R">2�ڂ̋��̔��a</param>
	/// <returns></returns>
	static bool Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, 
						  const float& sphereR, const float& sphere2R);

	/// <summary>
	/// obj�ƍU���̓����蔻��
	/// </summary>
	/// <param name="obj">���肵�����I�u�W�F�N�g</param>
	/// <param name="attack">���肵�����U��</param>
	/// <returns></returns>
	static bool Attack2OBJ(Object* obj, AttackBase* attack);
};

