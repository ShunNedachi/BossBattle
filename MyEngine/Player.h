#pragma once
#include"Object.h"
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include<time.h>
#include<vector>


class Player
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	Player(MyWindow* myw, MyDirectX12* my12,std::vector<Object> num);
	~Player();


	void Update(Input* input);
	void Draw();

	bool GetTurnEnd() { return turnEnd; }
	void SetTurnEnd(bool turnEnd) { this->turnEnd = turnEnd; }

	// ��M�����m�F�p
	void SetRecieveDamage(bool recieveDamage) { this->recieveDamage = recieveDamage; }

	// ���M�����m�F�p
	void SetSendDamage(bool sendDamage) { this->sendDamage = sendDamage; }
	bool GetSendDamage() { return sendDamage; }

	void HitDamage(int damage);
	int GetTotalDamage() { return totalDamage; }

	bool GetisDead() { return isDead; }

	void InitForLoop();

private:

	Object player;

	Sprite2D attackReadySprite{0.5f, 0.5f};
	Sprite2D blockReadySprite{0.5f,0.5f};
	Sprite2D timeoverSprite{ 0.5f,0.5f };

	Sprite2D greatSprite{ 0.5f,0.5f };
	Sprite2D goodSprite{ 0.5f,0.5f };
	Sprite2D badSprite{ 0.5f,0.5f };

	Sprite2D isRendaSprite{ 0.5f,0.5f };

	XMFLOAT3 scale = {3,3,3};
	XMFLOAT3 position = {-70,-40,0};
	XMFLOAT3 rotation = {0,180,0};

	std::vector<Object> number;

	// �X�v���C�g�o���p
	bool attackReady = true;
	bool blockReady = false;

	clock_t spriteStart;
	bool timeStart = true;

	// ��{�X�e�[�^�X
	float power = 10; // ��b�U����
	int HP = 50; // �̗�
	bool isDead = false;
	
	bool turnEnd = false;

	// �_���[�W����M�p
	bool sendDamage = false;
	bool recieveDamage = false;

	// �_���[�W����������Ƃ�
	int hitDamage = 0;
	bool recieveDamageEffect = false;
	// �_���[�W���󂯂����̃G�t�F�N�g�p

	bool damageToMoveXplus = true;
	clock_t recieveDamageTime;
	// ��������_���[�W�l1����������
	std::vector<int> damageNum;
	bool damageNumInit = false;



	// �U���p

	bool isAttack = false; // �U������
	float totalDamage = 0; // �{���̂��������_���[�W�l

	Sprite2D isAttackSprite{ 0.5f,0.5f };

	clock_t attackTimeStart;
	clock_t timeoverStart;
	bool timeover = false;
	bool timeoverInit = true;

	int attackNum = 0;
	enum attackPatern
	{
		renda,
		timingButton,
		sharpTiming,

	};



	// �_���[�W�^�C�~���O
	bool isDamage = false;
	bool damageMove = true;
	bool attackDamageDraw = false;
	bool playerToMoveZ = true;

	clock_t damageStart;

	// �A�ŗp
	const float rendaSuccessNum = 40;
	float nowRendaNum = 0;

	bool attackEnemyDraw = false; // �G�ւ̍U�����o�p
};

