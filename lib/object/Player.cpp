#include "Player.h"
#include"SpriteLoadDefine.h"

// �ÓI�ϐ��̎���
Player* Player::instance = nullptr;

Player* Player::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Player();
	}

	return instance;
}

void Player::Destroy()
{
	if (objPlayer != nullptr)delete objPlayer;
	objPlayer = nullptr;

	if (attack != nullptr)delete attack;
	attack = nullptr;

	// �̗�
	delete healthBarSprite;
	healthBarSprite = nullptr;
	for (auto x : healthSprite)
	{
		delete x;
		x = nullptr;
	}

	delete instance;
	instance = nullptr;
}

void Player::Init(const String& filename)
{
	// �v���C���[�̃I�u�W�F�N�g�p
	objPlayer = new Object(NORMAL,filename);

	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };

	objPlayer->SetScale(scale);
	objPlayer->SetRadius(1);
	// �����܂�

	// �̗͗p
	healthBarSprite = new Sprite2D(0.5f, 0.5f);
	healthBarSprite->CreateSprite(playerHealthBar);

	healthBarSprite->SetPosition({ 130,50 });
	for (int i = 0; i < health; i++)
	{
		Sprite2D* temp = new Sprite2D(0.5f,0.5f);
		temp->CreateSprite(playerHealth);
		temp->SetPosition({ (float)40 + i * 20,50 });

		healthSprite.push_back(temp);
	}
	// �����܂�

	// �U���֌W

	attack = new AttackBase(0, 10, 10);
	attackObj = new Object(NORMAL,"sphere");
	attackObj->SetScale({ 2,2,2 });
	attackObj->SetRadius(2);

	attack->SetObject(attackObj);
	// �����܂�
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	
	const bool XINPUT_TRIGGER_A = xinput->TriggerButtom(0, xinput_A);

	// �ړ������@��Ŋ֐������ď����𕪂���₷��
	const int XINPUT_MOVE_STICK = xinput->MoveStick(0, xinput_LS);

	// ���͊m�F
	if (XINPUT_MOVE_STICK & XINPUT_STICK_UP ||
		XINPUT_MOVE_STICK & XINPUT_STICK_DOWN||
		XINPUT_MOVE_STICK & XINPUT_STICK_LEFT ||
		XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
	{
		//	�U��������Ȃ��Ƃ��Ɉړ��\
		if (!attack->GetAttackStart())
		{
			move = {0,0,0};
			XMFLOAT3 attackPos = position;
			bool isMoveRIGHTLEFT = false;

			// �O�ςɕK�v�ȕϐ��̒�`
			XMFLOAT3 eyeDir = Camera::GetEyeDir();
			XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
			XMVECTOR upV = { Camera::GetUp().x,Camera::GetUp().y,Camera::GetUp().z,0 };
			// �v���C���[�̈ʒu����up�x�N�g�����Ƃ�
			XMVECTOR playerUpV = {position.x,position.y,position.z};
			playerUpV.m128_f32[1] += 1;
			// eyeDirV �Ɓ@up�̊O��
			// ���E�ړ��p
			XMVECTOR moveRIGHTLEFT = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
			// �㉺�ړ��p
			XMVECTOR moveUPDOWN = DirectX::XMVector3Normalize(DirectX::XMVector3Cross( moveRIGHTLEFT, playerUpV));

			// ����pos��ύX���āA�Ƃ肠�������������Ɉړ�
			if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT || XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
			{
				isMoveRIGHTLEFT = true;

				if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT)
				{
					move.m128_f32[0] -= moveRIGHTLEFT.m128_f32[0];
					move.m128_f32[2] -= moveRIGHTLEFT.m128_f32[2];
				}
				else if (XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
				{
					move.m128_f32[0] += moveRIGHTLEFT.m128_f32[0];
					move.m128_f32[2] += moveRIGHTLEFT.m128_f32[2];
				}
			}

			// �㉺�ړ����Ă���̂Ȃ�
			if (isMoveRIGHTLEFT)
			{
				move = DirectX::XMVector3Normalize(move);

				position.x += move.m128_f32[0];
				position.z += move.m128_f32[2];

				attackPos.x += move.m128_f32[0] * 5;
				attackPos.z += move.m128_f32[2] * 5;
				// ���E�ړ��p�ɏ�����
				move = { 0,0,0 };
			}

			if (XINPUT_MOVE_STICK & XINPUT_STICK_UP)
			{
				move.m128_f32[0] += moveUPDOWN.m128_f32[0];
				move.m128_f32[2] += moveUPDOWN.m128_f32[2];
			}
			else if (XINPUT_MOVE_STICK & XINPUT_STICK_DOWN)
			{
				move.m128_f32[0] -= moveUPDOWN.m128_f32[0];
				move.m128_f32[2] -= moveUPDOWN.m128_f32[2];
			}

			
			move =DirectX::XMVector3Normalize(move);

			position.x += move.m128_f32[0];
			position.z += move.m128_f32[2];

			attackPos.x += move.m128_f32[0] * 5;
			attackPos.z += move.m128_f32[2] * 5;

			objPlayer->SetPosition(position);
			attackObj->SetPosition(attackPos);

		}
	}
	// �����܂�

	//// ��]�ݒ�p �@y��-90���Ă���̂̓��f���̏����l�ɑ΂�����̂��ƂŕύX
	//// �U�����͉�]���~�߂�
	if(!attack->GetAttackStart())attackObj->SetRotation({ 0, -1 *Camera::GetPhi()- 90,0 });




	// �U���p����
	
	if (XINPUT_TRIGGER_A)
	{
		// �U���p�����ڃI�u�W�F�N�g�̈ʒu�ύX
		XMFLOAT3 eyeDir = Camera::GetEyeDir();
		// �����ύX�p
		const float DISTANCE = 5;

		//XMFLOAT3 tempPos = { position.x + move.m128_f32[0] * DISTANCE,position.y,position.z + move.m128_f32[2] * DISTANCE};
		//attackObj->SetPosition(tempPos);

		// ���ʕ����Ɍ��������킹��(���)

		attack->SetAttackStart();
	}

	// �����܂�


	// �_���[�W��Ԏ��̏���
	if (isDamage)
	{
		damageCount++;
		objPlayer->SetColor({ 0,1,1 });

		// �_���[�W��ԉ�������
		if (damageCount >= DAMAGE_FRAME)
		{
			isDamage = false;
			damageCount = 0;
			objPlayer->SetColor({ 1,1,1 });
		}

	}


	//	�X�V

	objPlayer->Update();

	attackObj->Update();
	attack->Update();
}

void Player::Draw()
{
	// ��Ń_���[�W��Ԃ̎��ɐԂ��_�ł���悤�ɏC������
	objPlayer->Draw();

	if (attack->GetIsAttack())attackObj->Draw();

	for (int i = 0; i < health; i++)
	{
		healthSprite[i]->Draw();
	}

	healthBarSprite->Draw();
}

void Player::RecieveDamage(XMFLOAT3 pos, float damage)
{
	// 
	isDamage = true;
	health -= damage;

	// �_���[�W���󂯂����Ɉʒu�����炷
	XMVECTOR movePos;
	// �ړ��p�@�Ƃ肠����xz�݈̂ړ�
	movePos.m128_f32[0] = (pos.x - position.x);
	movePos.m128_f32[2] = (pos.z - position.z);

	//movePos = DirectX::XMVector4Normalize(movePos);

	// �m�b�N�o�b�N����
	position.x -= movePos.m128_f32[0]* 10;
	position.z -= movePos.m128_f32[2]* 10;

	objPlayer->SetPosition(position);
}
