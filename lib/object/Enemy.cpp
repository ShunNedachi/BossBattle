#include "Enemy.h"
#include"GameFunction.h"

void Enemy::Init()
{
	// ���p�̃I�u�W�F�N�g�̐���

	obj = new Object(NORMAL,"scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(2.5f);
}

void Enemy::Update()
{
	//	player���K�E�Z���̂Ƃ�������~�߂�
	if (!GameFunction::GetPlayerIsSpecial())
	{
		const float SPEED = 0.1f;

		// �G���G�p�s���@�v���C���[�ɒǏ]
			// �Ǐ]�p�Ƀv���C���[�̈ʒu������Ă���
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		// �ړ�����x�N�g���̌v�Z (y�͂Ƃ肠�����Œ�l)
		XMVECTOR moveV = { playerPos.x - position.x,0,playerPos.z - position.z };
		moveV = DirectX::XMVector3Normalize(moveV);

		position.x += moveV.m128_f32[0] * SPEED;
		position.z += moveV.m128_f32[2] * SPEED;



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

void Enemy::RecieveDamage(XMFLOAT3 pos, float damage,bool knockback)
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
	if (knockback)
	{
		position.x -= movePos.m128_f32[0] * 10;
		position.z -= movePos.m128_f32[2] * 10;

		obj->SetPosition(position);
	}
}
