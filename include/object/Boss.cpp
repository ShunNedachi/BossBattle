#include "Boss.h"
#include"GameFunction.h"
#include"SpriteLoadDefine.h"
#include"Setting.h"

void Boss::Init()
{
	// ���p�̃I�u�W�F�N�g�̐���

	obj = new Object(NORMAL, "scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(2.5f);

	//health = MAX_HEALTH;

	hpSprite = new Sprite2D(0, 0.5f);
	hpSprite->CreateSprite(bossHealth);
	//hpSprite->Resize(ONE_HEALTH_SIZE * MAX_HEALTH, 50);
	hpSprite->Resize(0, 50);
	hpSprite->SetPosition({ WINDOW_WIDTH / 2 - 400,650 });

	hpBarSprite = new Sprite2D(0.5f, 0.5f);
	hpBarSprite->CreateSprite(bossHealthBar);
	hpBarSprite->Resize(ONE_HEALTH_SIZE * MAX_HEALTH, 50);
	hpBarSprite->SetPosition({ WINDOW_WIDTH / 2 ,650 });

}

void Boss::Update()
{
	// enemy�̒��g��null�����邩�̊m�F
	for (int i = 0; i < enemys.size(); i++)
	{
		// �G�̗̑͂�0�ȉ��ɂȂ��Ă����Ƃ��ɔz�񂩂�폜
		if (enemys[i]->GetHealth() <= 0)
		{
			GameFunction::AddEXP(enemys[i]->GetEXP());

			delete enemys[i];
			enemys[i] = nullptr;
			enemys.erase(enemys.begin() + i);
			i--;
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

	// �Q�[���J�n����̏��
	if (!isInit)
	{
		if (actionCount > initFrame)
		{
			isInit = true;
			actionCount = 0;
		}

		if (health < MAX_HEALTH)
		{
			health += 0.05f;
			hpSprite->Resize(ONE_HEALTH_SIZE * health, 50);
		}

		actionCount++;
	}
	else
	{
		if (!GameFunction::GetPlayerIsSpecial())
		{
			Action();

			// �_���[�W��Ԏ��̏���
			if (isDamage)
			{
				damageCount++;
				obj->SetColor({ 0,1,1 });

				hpSprite->Resize(ONE_HEALTH_SIZE * health, 50);

				// �_���[�W��ԉ�������
				if (damageCount >= DAMAGE_FRAME)
				{
					isDamage = false;
					damageCount = 0;
					obj->SetColor({ 1,1,1 });
				}
			}

			for (int i = 0; i < enemys.size(); i++)
			{
				enemys[i]->Update();
			}
		}
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

	hpSprite->Draw();
	hpBarSprite->Draw();
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

		// �W�����v�U���p
	case BossPattern::FlyCombo:

		result = ActionFallConmbo();
		break;

	default:
		break;
	}

	// �s���ύX���@�����ɂ���ăp�^�[���ύX
	if (result)
	{
		// debug�p�Ɉ�U�R�����g
		
		// �s���p�^�[���̕ύX�����������ڍׂ�
		//if (pattern != BossPattern::Bless)pattern = (BossPattern)((int)pattern + 1);
		//else pattern = BossPattern::Stop;

		// player�Ƃ̋����ɉ����ĕύX
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		// �v�Z�p
		const float DIS_X = playerPos.x - position.x;
		const float DIS_Y = playerPos.y - position.y;
		const float DIS_Z = playerPos.z - position.z;

		float distance = sqrt(DIS_X * DIS_X + DIS_Y * DIS_Y + DIS_Z * DIS_Z);

		// ���������ȏ㗣��Ă���Ƃ�
		if (distance > 20)
		{
			// �̗͂������ȉ��̎�
			if (health < MAX_HEALTH / 2)
			{
				int randNum = rand() % 5;

				if (randNum == 0) pattern = BossPattern::PopEnemy;
				else if (randNum == 1)pattern = BossPattern::Sleep;
				else if (randNum == 2)pattern = BossPattern::Rush;
				else if (randNum == 3)pattern = BossPattern::Bless;
				else if (randNum == 4)pattern = BossPattern::FlyCombo;

			}
			else
			{
				int randNum = rand() % 4;

				if (randNum == 0) pattern = BossPattern::PopEnemy;
				else if (randNum == 1)pattern = BossPattern::Rush;
				else if (randNum == 2)pattern = BossPattern::Bless;
				else if (randNum == 3)pattern = BossPattern::FlyCombo;

			}
		}
		else
		{
			// �̗͂������ȉ��̎�
			if (health < MAX_HEALTH / 2)
			{
				int randNum = rand() % 4;

				if (randNum == 0) pattern = BossPattern::PopEnemy;
				else if (randNum == 1)pattern = BossPattern::Sleep;
				else if (randNum == 2)pattern = BossPattern::Follow;
				else if (randNum == 3)pattern = BossPattern::Rush;

			}
			else
			{
				int randNum = rand() % 3;

				if (randNum == 0) pattern = BossPattern::PopEnemy;
				else if (randNum == 2)pattern = BossPattern::Follow;
				else if (randNum == 3)pattern = BossPattern::Rush;

			}
		}
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

			tempPos.x += blessV[i].x * 2;
			tempPos.y += blessV[i].y * 2;
			tempPos.z += blessV[i].z * 2;

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

		//blessV.clear();
		//blessV.shrink_to_fit();
	}

	return endFlg;
}


bool Boss::ActionFallConmbo()
{
	bool endFlg = false;

	// �U���֌W����������
	if (initAttack)
	{
		// easing�p
		flyEasing.SetState(2, position.y, MAX_FLY_POS);

		startFly = true;
		// ����������false��
		initAttack = false;
		initEndFly = true;
	}
	
	// �J�n���̏���
	if (startFly && !endFly)
	{
		position.y = flyEasing.StartEeaging(easeInFLAG);
		
		if (flyEasing.GetEndFlg())
		{
			startFly = false;
		}
	}
	else if(!startFly && !endFly)// �U������
	{
		// �ƂɋA���Ă������
		// �㏸������U���Ă��鏈��
		const float FLYATTACK_TIMING = actionCount % flyAttackFrame;
		if (FLYATTACK_TIMING == 0 && !flyAttackFlg)
		{
			flyAttackFlg = true;
			flyBack = false;

			// ���������̐ݒ�
			XMFLOAT3 playerPos = GameFunction::GetPlayerPos();
			flyComboV = { playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z };
			flyComboV = DirectX::XMVector3Normalize(flyComboV);
			
		}

		if (flyAttackFlg)
		{
			if (!flyBack)
			{

				const float SPEED = 5;

				// �~���s��
				position.x += flyComboV.m128_f32[0] * SPEED;
				position.y += flyComboV.m128_f32[1] * SPEED;
				position.z += flyComboV.m128_f32[2] * SPEED;

				if (position.y <= 0) 
				{
					flyBack = true; 
					flyEasing.SetState(1, position.y, MAX_FLY_POS);
				}
			}
			else
			{
				position.y = flyEasing.StartEeaging(easeOutFLAG);

				if (flyEasing.GetEndFlg())
				{
					flyAttackFlg = false;
				}
			}
		}


		actionCount++;
	}




	// ���Ԃ��o�߂�����I���t���O��true
	if (actionCount >= flyFrame && initEndFly && position.y >= MAX_FLY_POS)
	{
		endFly = true;
		flyEasing.SetState(2, position.y, 0);
		initEndFly = false;
	}

	// �I�����̒�������
	if (endFly)
	{
		position.y = flyEasing.StartEeaging(easeOutFLAG);
		
		if (flyEasing.GetEndFlg())
		{
			endFlg = true;
		}
	}

	// �I�������ɒB���Ă�����
	if (endFlg)
	{

		// ������Ԃɖ߂�
		endFly = false;
		initEndFly = true;
		actionCount = 0;
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
