#include "Collision.h"
#include"AttackBase.h"

bool Collision::Sphere2Sphere(const DirectX::XMFLOAT3& sphere1Pos, const DirectX::XMFLOAT3& sphere2Pos, const float& sphereR, const float& sphere2R)
{
	// åvéZópç∂ï”
	DirectX::XMFLOAT3 tempPos = { sphere1Pos.x - sphere2Pos.x,sphere1Pos.y - sphere2Pos.y, sphere1Pos.z - sphere2Pos.z };
	float distance = sphereR + sphere2R;
	// îªíËópÇ…2èÊÇ∑ÇÈ
	tempPos.x *= tempPos.x;
	tempPos.y *= tempPos.y;
	tempPos.z *= tempPos.z;

	distance *= distance;


	// è’ìÀîªíË
	if (tempPos.x + tempPos.y + tempPos.z <= distance)
	{
		return true;
	}

	return false;
}

bool Collision::Attack2OBJ(Object* obj,AttackBase* attack)
{
	AttackBase* tempAttack = attack;
	Object* tempObj = obj;
	DirectX::XMFLOAT3 position = tempObj->GetPosition();
	DirectX::XMFLOAT3 size = tempObj->GetSize();
	float r = tempObj->GetRadius();


	if (tempAttack->Hit(position,size,r))
	{
		return true;
	}

	return false;
}
