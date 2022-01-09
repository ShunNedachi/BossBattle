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
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1Pos">1つ目の球の位置</param>
	/// <param name="sphere2Pos">2つ目の球の位置</param>
	/// <param name="sphereR">1つ目の球の半径</param>
	/// <param name="sphere2R">2つ目の球の半径</param>
	/// <returns></returns>
	static bool Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, 
						  const float& sphereR, const float& sphere2R);


	#pragma region 2D系の当たり判定


	static bool Point2Point_2D(const Point2D& p1,const Point2D& p2);

	static bool Point2Segment2D(const Point2D& point, const Segment2D& segment);



	#pragma endregion



	// 作成したクラスとの判定

	/// <summary>
	/// objと攻撃の当たり判定
	/// </summary>
	/// <param name="obj">判定したいオブジェクト</param>
	/// <param name="attack">判定したい攻撃</param>
	/// <returns></returns>
	static bool Attack2OBJ(const Object& obj,const AttackBase& attack);


	/// <summary>
	/// playerとenemyの当たり判定用
	/// </summary>
	/// <param name="player">playerの情報</param>
	/// <param name="enemy">enemyの情報</param>
	/// <param name="knockback">playerがノックバックするのかどうか</param>
	static void Player2Enemy(Player& player,Enemy& enemy,bool knockback);

	static bool Player2SphereOBJ(Player& player,Object& obj,float damage);
};

