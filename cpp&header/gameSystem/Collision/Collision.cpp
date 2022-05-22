#include "Collision.h"
#include"AttackBase.h"

bool Collision::Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, const float& sphereR, const float& sphere2R)
{
	// �v�Z�p����
	DirectX::XMFLOAT3 tempPos = { sphere1Pos.x - sphere2Pos.x,sphere1Pos.y - sphere2Pos.y, sphere1Pos.z - sphere2Pos.z };
	float distance = sphereR + sphere2R;
	// ����p��2�悷��
	tempPos.x *= tempPos.x;
	tempPos.y *= tempPos.y;
	tempPos.z *= tempPos.z;

	distance *= distance;


	// �Փ˔���
	if (tempPos.x + tempPos.y + tempPos.z <= distance)
	{
		return true;
	}

	return false;
}


#pragma region 2D�p����

bool Collision::Point2Point_2D(const Point2D& p1, const Point2D& p2)
{
	// �덷�͈͓��Ȃ画��
	const float E = 0.00001f;

	float disP1 = p1.x - p2.x;
	float disP2 = p1.y - p2.y;

	if (disP1 < 0)disP1 * -1;
	if (disP2 < 0)disP2 * -1;

	// �������덷�͈͓��Ȃ瓖�����Ă���
	if (disP1 < E && disP2 < E) return true;

	// �������Ă��Ȃ��Ƃ�
	return false;
}

bool Collision::Point2Segment2D(const Point2D& point, const Segment2D& segment)
{
	// �덷�͈͓��Ȃ画��
	const float E = 0.00001f;


	// �v�Z�p
	Vector2 temp(segment.point.x - point.x, segment.point.y - point.y);


	// �덷�v�Z�p
	float dot = temp.Dot(segment.dirV);

	if (dot < 0)dot *= -1;

	bool result = false;

	// �덷�ȓ��Ȃ炠�����Ă���
	if (dot <= E)result = true;

	return result;
}

#pragma endregion


bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane)
{
	const float epsilon = 1.0e-5f; // �덷�͈͗p

	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	DirectX::XMVECTOR normal = { plane.normal.x,plane.normal.y,plane.normal.z };
	DirectX::XMVECTOR rayDir = { ray.dirVector.x,ray.dirVector.y,ray.dirVector.z };

	float d1 = DirectX::XMVector3Dot(normal, rayDir).m128_f32[0];
	if (d1 > -epsilon) { return false; }

	// �ʖ@���ƃ��C�̎n�_���W�Ƃ̓���
	DirectX::XMVECTOR rayStart = { ray.point.x,ray.point.y,ray.point.z };
	float d2 = DirectX::XMVector3Dot(normal, rayStart).m128_f32[0];

	// �n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;
	// �n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	// ��_���n�_�����ɂ���̂œ�����Ȃ�
	if (t < 0) { return false; }

	return true;

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
	// �����蔻��p�@�萔�l
	// �ʒu���̊l��
	const DirectX::XMFLOAT3 PLAYER_POS = player.GetPos();
	const DirectX::XMFLOAT3 ENEMY_POS = enemy.GetPosition();
	const AttackBase* PLAYER_ATTACK = player.GetAttack();
	const AttackBase* PLAYER_SPECIAL_ATTACK = player.GetSpecialAttack();
	const DirectX::XMFLOAT3 PLAYER_ATTACK_POS = player.GetAttackPos();
	const Object* ENEMY_OBJ = enemy.GetOBJ();
	// �T�C�Y�p
	const float PLAYER_RADIUS = player.GetRadius();
	const float ENEMY_RADIUS = enemy.GetRadius();
	// �_���[�W�@��ԁ@�ʂ̎擾
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

bool Collision::Player2SphereOBJ(Player& player,Object& obj,float damage)
{
	// ���񂱂�����G�̉������U���Ƃ̔�������
	DirectX::XMFLOAT3 playerPos = player.GetPos();
	DirectX::XMFLOAT3 objPos = obj.GetPosition();
	float playerSize = player.GetRadius();
	float objSize = obj.GetRadius();
	const bool PLAYER_ISDAMAGE = player.GetIsDamage();

	if (Collision::Sphere2Sphere(playerPos, objPos, playerSize, objSize))
	{
		if (!PLAYER_ISDAMAGE)
		{
			player.RecieveDamage(objPos, damage);
		}

		return true;
	}


	return false;
}