#include "Boss.h"
#include"GameFunction.h"

void Boss::Init()
{
	// ���p�̃I�u�W�F�N�g�̐���

	obj = new Object(NORMAL, "scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(2.5f);

	health = 10;
}

void Boss::Update()
{
	// enemy�̒��g��null�����邩�̊m�F
	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys[i]->GetHealth() <= 0)
		{
			GameFunction::AddEXP(enemys[i]->GetEXP());

			delete enemys[i];
			enemys[i] = nullptr;
			enemys.erase(enemys.begin() + i);
		}
	}
	// attackObjs�̒��g��null�����邩�̊m�F
	for (auto itr = attackObjs.begin(); itr < attackObjs.end();)
	{
		if (*itr == nullptr)
		{
			itr = attackObjs.erase(itr);
		}
		else
		{
			itr++;
		}

	}


	if (!GameFunction::GetPlayerIsSpecial())
	{
		Action();

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

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Update();
	}
	obj->Update();
	obj->SetPosition(position);
}

void Boss::Draw()
{
	obj->Draw();

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Draw();
	}

	for (int i = 0; i < attackObjs.size(); i++)
	{
		attackObjs[i]->Draw();
	}
}

void Boss::Destroy()
{
	if (enemys.size() > 0)
	{
		for (auto& x : enemys)
		{
			delete x;
			x = nullptr;
		}

		enemys.clear();
		enemys.shrink_to_fit();
	}

	DeleteAttack();

	delete obj;
	obj = nullptr;
}

void Boss::AddEnemy(XMFLOAT3 position, XMFLOAT3 scale,XMFLOAT3 rotation)
{
	Enemy* temp = new Enemy();
	temp->Init();
	temp->SetPosition(position);
	temp->SetScale(scale);
	temp->SetRotation(rotation);
	temp->SetRadius(scale.x / 2);

	enemys.push_back(temp);
}

void Boss::DeleteAttack()
{

}

// AI����
void Boss::Action()
{
	bool result = false;

	// �s���p�^�[������p
	switch (pattern)
	{
		// ��~���
	case BossPattern::Stop:

		result = ActionStop();
		break;

		// �G�o�����
	case BossPattern::PopEnemy:

		result = ActionPopEnemy();
		break;

		// �x�e���
	case BossPattern::Sleep:

		result = ActionSleep();
		break;

		// �Ǐ]���
	case BossPattern::Follow:

		result = ActionFollow();
		break;

		// �ːi���
	case BossPattern::Rush:

		result = ActionRush();
		break;

		//�@�������U�����
	case BossPattern::Bless:

		result = ActionBless();
		break;

	default:
		break;
	}

	// �s���ύX���@�����ɂ���ăp�^�[���ύX
	if (result)
	{
		// debug�p�Ɉ�U�R�����g

		//if (pattern != BossPattern::Bless)pattern = (BossPattern)((int)pattern + 1);
		//else pattern = BossPattern::Stop;
	}
}

// ��~���
bool Boss::ActionStop()
{
	bool endFlg = false;

	// ��莞�Ԍo�߂�����I���t���Otrue
	if (actionCount >= stopFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// �G�o�����
bool Boss::ActionPopEnemy()
{
	bool endFlg = false;

	// ��莞�Ԍo�߂�����I��
	if (actionCount >= popFrame)
	{
		endFlg = true;

		// �ʒu�͌�Œ���
		AddEnemy();
		AddEnemy({ 10,0,10 });
		AddEnemy({ 10,0,-10 });

		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// �x�e���
bool Boss::ActionSleep()
{
	bool endFlg = false;


	// ��莞�Ԍo�߂�����I��
	if (actionCount >= sleepFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	const int HEAL_TIMING =  actionCount % healFrame;

	// �񕜂̃^�C�~���O�ɂȂ������
	if (HEAL_TIMING == 0)
	{
		if (health <= 5)health += 5;
		else if (health > 6)health = 10;
	}
	return endFlg;
}

// �Ǐ]���
bool Boss::ActionFollow()
{
	bool endFlg = false;
	// �Ǐ]�p�Ƀv���C���[�̈ʒu������Ă���
	XMFLOAT3 playerPos =  GameFunction::GetPlayerPos();

	// �Ǐ]�O�̑ҋ@���Ԃ��������Ƃ��ɒǏ]
	if(actionCount > followStopFrame)
	{
		const float SPEED = 0.6f;

		// �ړ�����x�N�g���̌v�Z (y�͂Ƃ肠�����Œ�l)
		XMVECTOR moveV = { playerPos.x - position.x,0,playerPos.z - position.z };
		moveV = DirectX::XMVector3Normalize(moveV);

		position.x += moveV.m128_f32[0] * SPEED;
		position.z += moveV.m128_f32[2] * SPEED;

	}

	// ��莞�Ԍo�߂�����I��
	if (actionCount >= followFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// �ːi���
bool Boss::ActionRush()
{	
	bool endFlg = false;

	rushCount++;

	// �ːi�̑ҋ@���Ȃ�
	if (rushCount >= rushWaitFrame && !isRush)
	{
		isRush = true;
		rushCount = 0;

		// �Ǐ]�p�Ƀv���C���[�̈ʒu������Ă���
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		rushV = { playerPos.x - position.x, 0, playerPos.z - position.z };
		rushV = DirectX::XMVector3Normalize(rushV);
	}

	if (isRush)
	{
		// ��񓖂���̓ːi���Ԃ����o�ߎ��Ԃ����Ȃ�������
		if (oneRushFrame >= rushCount)
		{
			const float SPEED = 1.5f;

			position.x += rushV.m128_f32[0] * SPEED;
			position.z += rushV.m128_f32[2] * SPEED;
		}
		else
		{
			isRush = false;
			rushCount = 0;
		}
	}


	// ��莞�Ԍo�߂�����I��
	if (actionCount >= rushFrame)
	{
		endFlg = true;
		actionCount = 0;

		rushCount = 0;
		isRush = false;
	}
	else actionCount++;

	return endFlg;
}

bool Boss::ActionSpecial()
{
	bool endFlg = false;

	return endFlg;
}

bool Boss::ActionBless()
{
	bool endFlg = false;

	// �U���֌W����������
	//if (initAttack)
	//{
	//	// �U�����̐���
	//	attack = new AttackBase(1 * GAME_FRAME, 1 * GAME_FRAME, 10);
	//	// ����������false��
	//	initAttack = false;
	//}


	// �U������
	const int BLESS_TIMING = actionCount % blessWaitFrame;

	// bless�̃^�C�~���O�ɂȂ�����U��
	if (BLESS_TIMING == 0)
	{
		// �U�������̌���
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();
		XMVECTOR v = { playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z,0 };
		v = DirectX::XMVector3Normalize(v);
		// �����̌���
		blessV.push_back(XMFLOAT3(v.m128_f32[0],v.m128_f32[1],v.m128_f32[2]));

		// �U���I�u�W�F�N�g�̐���
		Object* temp = new Object(NORMAL,"sphere");
		temp->SetPosition(position);
		temp->SetRadius(0.5f);

		attackObjs.push_back(temp);

	}
	
	actionCount++;

	// �U���̈ړ�����
	if (attackObjs.size() > 0)
	{
		for (int i = 0; i < attackObjs.size(); i++)
		{
			XMFLOAT3 tempPos = attackObjs[i]->GetPosition();

			tempPos.x += blessV[i].x;
			tempPos.y += blessV[i].y;
			tempPos.z += blessV[i].z;

			attackObjs[i]->SetPosition(tempPos);
			attackObjs[i]->Update();
		}
	}

	//�@�U���t���[�����߂�����
	if (blessFrame <= actionCount)
	{
		endFlg = true;
		actionCount = 0;
	}

	// �I�������𖞂����Ă�����I�u�W�F�N�g������
	if (endFlg)
	{
		// �Ƃ肠�����I������̂��킩��悤�Ƀp�^�[�����I��������e������
		DestroyAttackArray();
	}

	return endFlg;
}

bool Boss::ActionLightning()
{
	bool endFlg = false;

	// �U���֌W����������
	if (initAttack)
	{


		// ����������false��
		initAttack = false;
	}

	return endFlg;
}

bool Boss::ActionFallConmbo()
{
	bool endFlg = false;

	// �U���֌W����������
	if (initAttack)
	{


		// ����������false��
		initAttack = false;
	}

	return endFlg;
}

bool Boss::ActionFlyBless()
{
	bool endFlg = false;

	// �U���֌W����������
	if (initAttack)
	{


		// ����������false��
		initAttack = false;
	}

	return endFlg;
}

void Boss::DestroyAttackArray()
{
	for (auto x : attackObjs)
	{
		delete x;
		x = nullptr;
	}

	attackObjs.clear();
	attackObjs.shrink_to_fit();

	blessV.clear();
	blessV.shrink_to_fit();
}
