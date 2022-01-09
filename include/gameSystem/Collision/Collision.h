#pragma once
#include"Object.h"
#include"Player.h"
#include"Enemy.h"
#include"Boss.h"
#include<DirectXMath.h>
#include"CollidePrimitive.h"

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


	#pragma region 2D�n�̓����蔻��


	static bool Point2Point_2D(const Point2D& p1,const Point2D& p2);

	static bool Point2Segment2D(const Point2D& point, const Segment2D& segment);



	#pragma endregion



	// �쐬�����N���X�Ƃ̔���

	/// <summary>
	/// obj�ƍU���̓����蔻��
	/// </summary>
	/// <param name="obj">���肵�����I�u�W�F�N�g</param>
	/// <param name="attack">���肵�����U��</param>
	/// <returns></returns>
	static bool Attack2OBJ(const Object& obj,const AttackBase& attack);


	/// <summary>
	/// player��enemy�̓����蔻��p
	/// </summary>
	/// <param name="player">player�̏��</param>
	/// <param name="enemy">enemy�̏��</param>
	/// <param name="knockback">player���m�b�N�o�b�N����̂��ǂ���</param>
	static void Player2Enemy(Player& player,Enemy& enemy,bool knockback);

	static bool Player2SphereOBJ(Player& player,Object& obj,float damage);
};

