#include "Enemy.h"

void Enemy::Init()
{
	// ���p�̃I�u�W�F�N�g�̐���

	obj = new Object(NORMAL,"scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(5);
}

void Enemy::Update()
{

	// �_���[�W��Ԏ��̏���
	if (isDamage)
	{
		damageCount++;
		obj->SetColor({ 0,1,1 });

		// �_���[�W��ԉ�������
		if (damageCount >= DAMAGE_FRAME)
		{
			isDamage = false;
			damageCount = 0;
			obj->SetColor({ 1,1,1 });
		}
	}

	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->Update();
}

void Enemy::Draw()
{
	obj->Draw();
}

void Enemy::Destroy()
{
	// ���p�̏���
	delete obj;
	obj = nullptr;
}

void Enemy::RecieveDamage(XMFLOAT3 pos, float damage)
{
	// 
	isDamage = true;
	health -= damage;

	// �_���[�W���󂯂����Ɉʒu�����炷
	XMVECTOR movePos{};
	// �ړ��p�@�Ƃ肠����xz�݈̂ړ�
	movePos.m128_f32[0] = (pos.x - position.x);
	movePos.m128_f32[2] = (pos.z - position.z);
	

	movePos = DirectX::XMVector3Normalize(movePos);

	// �m�b�N�o�b�N����
	position.x -= movePos.m128_f32[0] * 10;
	position.z -= movePos.m128_f32[2] * 10;

	obj->SetPosition(position);

}
