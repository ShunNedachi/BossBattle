#include "Player.h"
#include"Sprite2D.h"
#include<random>

Player::Player(MyWindow* myw,MyDirectX12* my12,std::vector<Object> num)
{

	#pragma region スプライト初期化

	Sprite2D::LoadTex(20, L"Resources/texture/attack.png");
	attackReadySprite.CreateSprite(20);
	attackReadySprite.SetPosition({ 600,100,0 });

	Sprite2D::LoadTex(21, L"Resources/texture/isattack.png");
	isAttackSprite.CreateSprite(21);
	isAttackSprite.SetPosition({ 600,100,0 });

	Sprite2D::LoadTex(22, L"Resources/texture/isRenda.png");
	isRendaSprite.CreateSprite(22);
	isRendaSprite.SetPosition({ 600,100,0 });

	Sprite2D::LoadTex(23, L"Resources/texture/timeover.png");
	timeoverSprite.CreateSprite(23);
	timeoverSprite.SetPosition({ 600,100,0 });

	Sprite2D::LoadTex(24, L"Resources/texture/Great.png");
	greatSprite.CreateSprite(24);
	greatSprite.SetPosition({ 210,350,0 });

	Sprite2D::LoadTex(25, L"Resources/texture/Good.png");
	goodSprite.CreateSprite(25);
	goodSprite.SetPosition({ 210,350,0 });

	Sprite2D::LoadTex(26, L"Resources/texture/Bad.png");
	badSprite.CreateSprite(26);
	badSprite.SetPosition({ 210,350,0 });

	#pragma endregion


	#pragma region オブジェクト初期化

	player.Init(my12, myw);
	player.CreateModel("player");


	number = num;
#pragma endregion
	
}
Player::~Player()
{
}



void Player::Update(Input* input)
{
	if (attackReady)
	{
		if (timeStart)
		{
			spriteStart = clock();
			timeStart = false;
		}
		clock_t endSprite = clock();

		if (((float)endSprite - spriteStart )/ CLOCKS_PER_SEC > 2)
		{
			attackReady = false;
			timeStart = true;
			isAttack = true;
			srand(time(NULL));
			//attackNum = rand() % 3;
			attackNum = renda;

			attackTimeStart = clock();
		}
	}

	if (isAttack)
	{
		clock_t endAttackTime = clock();

		if (attackNum == renda && !timeover)
		{
			if (input->TriggerAButtom())
			{
				position.y += 3;
				nowRendaNum++;
			}
			else
			{
				position.y = -40;
			}
		}

		if (((float)endAttackTime - attackTimeStart)/CLOCKS_PER_SEC > 5 && attackNum == renda && timeoverInit)
		{
			timeoverInit = false;
			timeover = true;
			timeoverStart = clock();
		}


		if (timeover && attackNum == renda)
		{
			clock_t timeoverTime = clock();
			if (((float)timeoverTime - timeoverStart)/CLOCKS_PER_SEC > 2)
			{
				timeover = false;
				timeoverInit = true;
				isAttack = false;
				isDamage = true;

				// ダメージ倍率計算
				totalDamage = power * (nowRendaNum / rendaSuccessNum);
				if (totalDamage > power) totalDamage = power;

				nowRendaNum = 0;
			}
		}



	}

	// ダメージを与えるとき
	if (isDamage)
	{
		// ダメージ時の移動
		if (damageMove)
		{
			if (playerToMoveZ)position.z++;
			else position.z--;

			if (!playerToMoveZ && position.z <= 0)
			{
				damageMove = false;
				attackDamageDraw = true;
				damageStart = clock();
				sendDamage = true;
				playerToMoveZ = true;

			}
			
			if (position.z > 10)playerToMoveZ = false;
		}


		// 攻撃ダメージ表記用
		if (attackDamageDraw)
		{
			clock_t damageDrawEnd = clock();



			// 終了フラグ
			if (((float)damageDrawEnd - damageStart) / CLOCKS_PER_SEC > 2)
			{
				attackDamageDraw = false;
				damageMove = true;
				isDamage = false;

			}
		}
	}
	
	if (HP <= 0 && !recieveDamageEffect)isDead = true;

	// ダメージを受けたときのエフェクト
	if (recieveDamageEffect)
	{
		clock_t effectEnd = clock();
		if (((float)effectEnd - recieveDamageTime) / CLOCKS_PER_SEC > 1)
		{
			recieveDamageEffect = false;
			damageToMoveXplus = true;
			attackReady = true;

			position.x = -70;
		}

		if (damageToMoveXplus)position.x++;
		else position.x--;

		if (position.x > -65)damageToMoveXplus = false;
		else if (position.x < -75)damageToMoveXplus = true;

	}
	if (damageNumInit)
	{
		if (damageNum.size() != 0)
		{
			damageNum.clear();
			damageNum.shrink_to_fit();
		}
		std::string damageString = std::to_string(hitDamage);

		for (int i = 0; i < damageString.length(); i++)
		{
			damageNum.push_back(atoi(damageString.substr(i, 1).c_str()));
		}
		damageNumInit = false;
	}


	player.Update(position, scale, rotation);
}

void Player::Draw()
{
	player.Draw(1);

	if (attackReady)attackReadySprite.Draw();
	if (isAttack)
	{
		if (timeover)timeoverSprite.Draw();
		if (attackNum == renda && !timeover)isRendaSprite.Draw();
	}

	if (isDamage && attackDamageDraw)
	{
		if(totalDamage >= 8)greatSprite.Draw();
		if (totalDamage >= 4 && totalDamage < 8)goodSprite.Draw();
		if (totalDamage < 3)badSprite.Draw();
	}

	if (recieveDamageEffect)
	{
		for (int i = 0; i < damageNum.size(); i++)
		{
			number[damageNum[i]].Update({ -70 + (5 * (float)i),0,0 }, number[i].GetScale());
			number[damageNum[i]].Draw(1);
		}
	}

}

void Player::HitDamage(int damage)
{
	hitDamage = damage;
	HP -= damage;
	recieveDamageEffect = true;

	recieveDamage = false;
	recieveDamageTime = clock();

	damageNumInit = true;
}

void Player::InitForLoop()
{
	attackReady = true;
	blockReady = false;
	timeStart = true;
	isDead = false;
	turnEnd = false;
	sendDamage = false;
	recieveDamage = false;
	hitDamage = 0;
	recieveDamageEffect = false;
	damageToMoveXplus = true;
	damageNumInit = false;
	isAttack = false; // 攻撃中か
	totalDamage = 0; // 倍率のかかったダメージ値
	timeover = false;
	timeoverInit = true;
	isDamage = false;
	damageMove = true;
	attackDamageDraw = false;
	playerToMoveZ = true;
	nowRendaNum = 0;
	attackEnemyDraw = false; // 敵への攻撃演出用
}