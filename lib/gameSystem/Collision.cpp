#include "Collision.h"
#include"AttackBase.h"

bool Collision::Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, const float& sphereR, const float& sphere2R)
{
	// 計算用左辺
	DirectX::XMFLOAT3 tempPos = { sphere1Pos.x - sphere2Pos.x,sphere1Pos.y - sphere2Pos.y, sphere1Pos.z - sphere2Pos.z };
	float distance = sphereR + sphere2R;
	// 判定用に2乗する
	tempPos.x *= tempPos.x;
	tempPos.y *= tempPos.y;
	tempPos.z *= tempPos.z;

	distance *= distance;


	// 衝突判定
	if (tempPos.x + tempPos.y + tempPos.z <= distance)
	{
		return true;
	}

	return false;
}

bool Collision::Attack2OBJ(const Object& obj,const AttackBase& attack)
{
	AttackBase tempAttack = attack;
	Object tempObj = obj;
	DirectX::XMFLOAT3 position = tempObj.GetPosition();
	DirectX::XMFLOAT3 size = tempObj.GetSize();
	float r = tempObj.GetRadius();


	if (tempAttack.Hit(position,size,r))
	{
		return true;
	}

	return false;
}

void Collision::Player2Enemy(Player& player,Enemy& enemy,bool knockback)
{
	// 当たり判定用　定数値
	// 位置情報の獲得
	const DirectX::XMFLOAT3 PLAYER_POS = player.GetPos();
	const DirectX::XMFLOAT3 ENEMY_POS = enemy.GetPosition();
	const AttackBase* PLAYER_ATTACK = player.GetAttack();
	const AttackBase* PLAYER_SPECIAL_ATTACK = player.GetSpecialAttack();
	const DirectX::XMFLOAT3 PLAYER_ATTACK_POS = player.GetAttackPos();
	const Object* ENEMY_OBJ = enemy.GetOBJ();
	// サイズ用
	const float PLAYER_RADIUS = player.GetRadius();
	const float ENEMY_RADIUS = enemy.GetRadius();
	// ダメージ　状態　量の取得
	const bool ENEMY_ISDAMAGE = enemy.GetIsDamage();
	const bool PLAYER_ISDAMAGE = player.GetIsDamage();
	const int PLAYER_DAMAGE = player.GetDamage();
	const float PLAYER_SPECIAL_DAMAGE = player.GetSpecialDamage();
	const int ENEMY_DAMAGE = enemy.GetDamage();

	// playerAttack 2 enemy
	if (Collision::Attack2OBJ(*ENEMY_OBJ, *PLAYER_ATTACK))
	{
		if (!ENEMY_ISDAMAGE)enemy.RecieveDamage(PLAYER_ATTACK_POS, PLAYER_DAMAGE,knockback);
	}
	else if (Collision::Attack2OBJ(*ENEMY_OBJ, *PLAYER_SPECIAL_ATTACK))
	{
		if (!ENEMY_ISDAMAGE)enemy.RecieveDamage(PLAYER_POS, PLAYER_SPECIAL_DAMAGE,knockback);
	}
	// player 2 enemy
	if (Collision::Sphere2Sphere(PLAYER_POS, ENEMY_POS, PLAYER_RADIUS, ENEMY_RADIUS))
	{
		if (!PLAYER_ISDAMAGE)player.RecieveDamage(ENEMY_POS, ENEMY_DAMAGE);
	}
}
