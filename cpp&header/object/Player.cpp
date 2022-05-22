#include "Player.h"
#include"SpriteLoadDefine.h"
#include"Setting.h"
#include<imgui.h>

// �ÓI�ϐ��̎���
Player* Player::instance = nullptr;
//float Player::experience = 0;

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

	if (attack != nullptr)
	{
		delete attack;
		delete attackObj;

		attack = nullptr;
		attackObj = nullptr;
	}

	if (specialAttack != nullptr)
	{
		delete specialAttack;
		delete specialAttackObj;

		specialAttack = nullptr;
		specialAttackObj = nullptr;
	}


	// �̗�
	delete healthBarSprite;
	healthBarSprite = nullptr;
	for (auto x : healthSprite)
	{
		delete x;
		x = nullptr;
	}

	// �o���l
	delete expBarSprite;
	expBarSprite = nullptr;
	for (auto x : expSprite)
	{
		delete x;
		x = nullptr;
	}

	// ���x��
	for (auto x : numberSprite)
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

	position = { 0,0,-30 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };

	objPlayer->SetPosition(position);
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

	// �o���l�p
	expBarSprite = new Sprite2D(0.5f, 0.5f);
	expBarSprite->CreateSprite(playerExpBar);
	expBarSprite->SetPosition({ 280,90 });

	// �ő�l��
	for (int i = 0; i < 100; i++)
	{
		Sprite2D* temp = new Sprite2D(0.5f, 0.5f);
		temp->CreateSprite(playerExp);
		temp->Resize(5, 40);
		temp->SetPosition({ (float)40 + i * 5,90 });

		expSprite.push_back(temp);
	}

	// ���x���p
	for (int i = 0; i < 10; i++)
	{
		Sprite2D* temp = new Sprite2D(0.5f, 0.5f);
		temp->CreateSprite(number0 + i);
		temp->SetPosition({(float)20, 90});

		numberSprite.push_back(temp);
	}


	// �����܂�

	// �U���֌W

	attack = new AttackBase(0, 10, 5);
	attackObj = new Object(NORMAL,"sphere");
	attackObj->SetScale({ 2,2,2 });
	attackObj->SetRadius(2);

	attack->SetObject(attackObj);

	// �K�E�Z�p
	specialAttack = new AttackBase(1.5f * SETTING_VALUE::GAME_FRAME, 1, 10);

	specialAttackObj = new Object(NORMAL, "sphere");
	specialAttackObj->SetScale({ 40,40,40 });
	specialAttackObj->SetRadius(40);
	specialAttack->SetObject(specialAttackObj);
	// �����܂�
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	
	const buttonState XINPUT_TRIGGER = xinput->TriggerButton(CONTROLLER_1);

	// �ړ������@��Ŋ֐������ď����𕪂���₷��
	const stickState XINPUT_MOVE_STICK = xinput->MoveStick(CONTROLLER_1, XINPUT_BUTTON_LS);


	// ���͊m�F
	if (XINPUT_MOVE_STICK & XINPUT_STICK_UP ||
		XINPUT_MOVE_STICK & XINPUT_STICK_DOWN||
		XINPUT_MOVE_STICK & XINPUT_STICK_LEFT ||
		XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT ||
		input->PushKey(DIK_W) || input->PushKey(DIK_S)||input->PushKey(DIK_A)|| input->PushKey(DIK_D))
	{
		//	�U��������Ȃ��Ƃ��Ɉړ��\
		if (!attack->GetAttackStart() && !specialAttack->GetAttackStart())
		{
			move = {0,0,0};
			bool isMoveRIGHTLEFT = false;
			const float SPEED = 0.5f;

			// �O�ςɕK�v�ȕϐ��̒�`
			XMFLOAT3 eyeDir = Camera::GetEyeDir();
			XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
			XMVECTOR upV = { Camera::GetUp().x,Camera::GetUp().y,Camera::GetUp().z,0 };

			// �v���C���[��up�x�N�g�����Ƃ�
			XMVECTOR playerUpV = { 0,1,0 };

			// eyeDirV �Ɓ@up�̊O��
			// ���E�ړ��p
			XMVECTOR moveRIGHTLEFT = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
			// �㉺�ړ��p
			XMVECTOR moveUPDOWN = DirectX::XMVector3Normalize(DirectX::XMVector3Cross( moveRIGHTLEFT, playerUpV));

			// ����pos��ύX���āA�Ƃ肠�������������Ɉړ�
			if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT || XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT||
				input->PushKey(DIK_A)||input->PushKey(DIK_D))
			{
				isMoveRIGHTLEFT = true;

				if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT || input->PushKey(DIK_A))
				{
					move.m128_f32[0] -= moveRIGHTLEFT.m128_f32[0];
					move.m128_f32[2] -= moveRIGHTLEFT.m128_f32[2];
				}
				else if (XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT|| input->PushKey(DIK_D))
				{
					move.m128_f32[0] += moveRIGHTLEFT.m128_f32[0];
					move.m128_f32[2] += moveRIGHTLEFT.m128_f32[2];
				}
			}

			// �㉺�ړ����Ă���̂Ȃ�
			if (isMoveRIGHTLEFT)
			{
				move = DirectX::XMVector3Normalize(move);

				position.x += move.m128_f32[0] * SPEED;
				position.z += move.m128_f32[2] * SPEED;

				// ���E�ړ��p�ɏ�����
				move = { 0,0,0 };
			}

			if (XINPUT_MOVE_STICK & XINPUT_STICK_UP || input->PushKey(DIK_W))
			{
				move.m128_f32[0] += moveUPDOWN.m128_f32[0];
				move.m128_f32[2] += moveUPDOWN.m128_f32[2];
			}
			else if (XINPUT_MOVE_STICK & XINPUT_STICK_DOWN || input->PushKey(DIK_S))
			{
				move.m128_f32[0] -= moveUPDOWN.m128_f32[0];
				move.m128_f32[2] -= moveUPDOWN.m128_f32[2];
			}

			
			move = DirectX::XMVector3Normalize(move);

			position.x += move.m128_f32[0] * SPEED;
			position.z += move.m128_f32[2] * SPEED;

			objPlayer->SetPosition(position);
			// �K�E�Z�̈ʒu��player�̈ʒu
			specialAttackObj->SetPosition(position);

		}
	}
	// �����܂�

	//// ��]�ݒ�p �@y��-90���Ă���̂̓��f���̏����l�ɑ΂�����̂��ƂŕύX
	//// �U�����͉�]���~�߂�
	//if(!attack->GetAttackStart())attackObj->SetRotation({ 0, -1 *Camera::GetPhi()- 90,0 });


	// �U���p����
	
	// �ʏ�U�� �{�^�����������Ƃ��@�K�E�Z���g�p���Ă��Ȃ��Ƃ�
	if (XINPUT_TRIGGER & XINPUT_BUTTON_A && !specialAttack->GetAttackStart() ||
		input->TriggerKey(DIK_SPACE) && !specialAttack->GetAttackStart())
	{
		// �U���p�����ڃI�u�W�F�N�g�̈ʒu�ύX
		XMFLOAT3 eyeDir = Camera::GetEyeDir();
		// �����ύX�p
		const float DISTANCE = 5;

		// �O�ςɕK�v�ȕϐ��̒�`
		XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
		XMVECTOR upV = { Camera::GetUp().x,Camera::GetUp().y,Camera::GetUp().z,0 };

		// �v���C���[�̈ʒu����up�x�N�g�����Ƃ�
		XMVECTOR playerUpV = { 0,1,0 };

		// eyeDirV �Ɓ@up�̊O��
		XMVECTOR side = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
		// �O�����v�Z�p
		XMVECTOR front = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(side, playerUpV));

		front = DirectX::XMVector3Normalize(front);

		float posX = front.m128_f32[0] * DISTANCE;
		float posZ = front.m128_f32[2] * DISTANCE;

		// �ʒu�̐ݒ�
		XMFLOAT3 setPos;
		setPos.x = position.x + posX;
		setPos.y = position.y;
		setPos.z = position.z + posZ;
		
		attackObj->SetPosition(setPos);
		
		attack->SetAttackStart();
	}
	else if (XINPUT_TRIGGER & XINPUT_BUTTON_Y && !attack->GetAttackStart() && level >= 0 ||
		input->TriggerKey(DIK_EQUALS) && !attack->GetAttackStart() && level >= 0) // �{�^�����������Ƃ��@�U�������Ă��Ȃ��Ƃ� ���x����0����Ȃ��Ƃ���
	{
		const float DAMAGE = 2.5f;
		
		// ���x��������ĈЗ͂��㏸
		specialDamage = DAMAGE * level + experience * 0.08f;
		level = 0;
		experience = 0;
		
		specialAttack->SetAttackStart();
		//Camera::GetInstance()->SetZoomState(5, 0.5f);
	}

	if (specialAttack->GetAttackStart())isSpecial = true;
	else isSpecial = false;

	// �����܂�


	// �_���[�W��Ԏ��̏���
	if (isDamage)
	{
		damageCount++;
		objPlayer->SetColor({ 1,0,0 });

		// �_���[�W��ԉ�������
		if (damageCount >= DAMAGE_FRAME)
		{
			isDamage = false;
			damageCount = 0;
			objPlayer->SetColor({ 1,1,1 });
		}

	}

	// ����ł邩�ǂ����̔���
	if (health <= 0)isDead = true;

	//	�X�V

	objPlayer->Update();

	attackObj->Update();
	attack->Update();

	specialAttack->Update();

}

void Player::Draw(Light& light)
{
	// ��Ń_���[�W��Ԃ̎��ɐԂ��_�ł���悤�ɏC������
	objPlayer->Draw(light);

	if (attack->GetIsAttack())attackObj->Draw(light);
	if (specialAttack->GetIsAttack())specialAttackObj->Draw(light);

	// �̗̓o�[�p
	for (int i = 0; i < health; i++){healthSprite[i]->Draw();}
	healthBarSprite->Draw();
	// �o���l�p
	expBarSprite->Draw();
	numberSprite[level]->Draw();
	for (int i = 0; i < experience; i++){expSprite[i]->Draw();}



#ifdef _DEBUG

	//ImGui::Begin("player Config");
	//ImGui::SetWindowSize(ImVec2(100, 300));
	//ImGui::End();

#endif // _DEBUG


}

void Player::RecieveDamage(XMFLOAT3 pos, float damage)
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
	position.x -= movePos.m128_f32[0]* 10;
	position.z -= movePos.m128_f32[2]* 10;

	objPlayer->SetPosition(position);
}

void Player::EXPupdate()
{
	const int MAX_LEVEL = 9;
	const float MAX_EXP = 100;

	while (true)
	{
		bool breakFlg = false;


		// �o���l�o�[�̏���𒴂�����0�ɖ߂� �ő僌�x���ɒB���Ă�����100�ɂ��ďI��
		if (experience >= MAX_EXP && level < MAX_LEVEL)
		{
			experience -= MAX_EXP;
			level++;
		}
		else if (experience >= MAX_EXP && level >= MAX_LEVEL)
		{
			experience = MAX_EXP;
			level = MAX_LEVEL;
		}
		else breakFlg = true;

		if (breakFlg)break;
	}

}
