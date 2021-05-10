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

	// 受信完了確認用
	void SetRecieveDamage(bool recieveDamage) { this->recieveDamage = recieveDamage; }

	// 送信完了確認用
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

	// スプライト出現用
	bool attackReady = true;
	bool blockReady = false;

	clock_t spriteStart;
	bool timeStart = true;

	// 基本ステータス
	float power = 10; // 基礎攻撃力
	int HP = 50; // 体力
	bool isDead = false;
	
	bool turnEnd = false;

	// ダメージ送受信用
	bool sendDamage = false;
	bool recieveDamage = false;

	// ダメージをくらったとき
	int hitDamage = 0;
	bool recieveDamageEffect = false;
	// ダメージを受けた時のエフェクト用

	bool damageToMoveXplus = true;
	clock_t recieveDamageTime;
	// くらったダメージ値1桁ずつを入れる
	std::vector<int> damageNum;
	bool damageNumInit = false;



	// 攻撃用

	bool isAttack = false; // 攻撃中か
	float totalDamage = 0; // 倍率のかかったダメージ値

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



	// ダメージタイミング
	bool isDamage = false;
	bool damageMove = true;
	bool attackDamageDraw = false;
	bool playerToMoveZ = true;

	clock_t damageStart;

	// 連打用
	const float rendaSuccessNum = 40;
	float nowRendaNum = 0;

	bool attackEnemyDraw = false; // 敵への攻撃演出用
};

