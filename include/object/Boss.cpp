#include "Boss.h"
#include"GameFunction.h"
#include"SpriteLoadDefine.h"
#include"Setting.h"

void Boss::Init()
{
	// 仮用のオブジェクトの生成

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
	// enemyの中身にnullがあるかの確認
	for (int i = 0; i < enemys.size(); i++)
	{
		// 敵の体力が0以下になっていたときに配列から削除
		if (enemys[i]->GetHealth() <= 0)
		{
			GameFunction::AddEXP(enemys[i]->GetEXP());

			delete enemys[i];
			enemys[i] = nullptr;
			enemys.erase(enemys.begin() + i);
			i--;
		}
	}
	// attackObjsの中身にnullがあるかの確認
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

	// ゲーム開始直後の状態
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

			// ダメージ状態時の処理
			if (isDamage)
			{
				damageCount++;
				obj->SetColor({ 0,1,1 });

				hpSprite->Resize(ONE_HEALTH_SIZE * health, 50);

				// ダメージ状態解除条件
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

// AI挙動
void Boss::Action()
{
	bool result = false;

	// 行動パターン判定用
	switch (pattern)
	{
		// 停止状態
	case BossPattern::Stop:

		result = ActionStop();
		break;

		// 敵出現状態
	case BossPattern::PopEnemy:

		result = ActionPopEnemy();
		break;

		// 休憩状態
	case BossPattern::Sleep:

		result = ActionSleep();
		break;

		// 追従状態
	case BossPattern::Follow:

		result = ActionFollow();
		break;

		// 突進状態
	case BossPattern::Rush:

		result = ActionRush();
		break;

		//　遠距離攻撃状態
	case BossPattern::Bless:

		result = ActionBless();
		break;

		// ジャンプ攻撃用
	case BossPattern::FlyCombo:

		result = ActionFallConmbo();
		break;

	default:
		break;
	}

	// 行動変更時　条件によってパターン変更
	if (result)
	{
		// debug用に一旦コメント
		
		// 行動パターンの変更をもう少し詳細に
		//if (pattern != BossPattern::Bless)pattern = (BossPattern)((int)pattern + 1);
		//else pattern = BossPattern::Stop;

		// playerとの距離に応じて変更
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		// 計算用
		const float DIS_X = playerPos.x - position.x;
		const float DIS_Y = playerPos.y - position.y;
		const float DIS_Z = playerPos.z - position.z;

		float distance = sqrt(DIS_X * DIS_X + DIS_Y * DIS_Y + DIS_Z * DIS_Z);

		// 距離が一定以上離れているとき
		if (distance > 20)
		{
			// 体力が半分以下の時
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
			// 体力が半分以下の時
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

// 停止状態
bool Boss::ActionStop()
{
	bool endFlg = false;

	// 一定時間経過したら終了フラグtrue
	if (actionCount >= stopFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// 敵出現状態
bool Boss::ActionPopEnemy()
{
	bool endFlg = false;

	// 一定時間経過したら終了
	if (actionCount >= popFrame)
	{
		endFlg = true;

		// 位置は後で調整
		AddEnemy();
		AddEnemy({ 10,0,10 });
		AddEnemy({ 10,0,-10 });

		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// 休憩状態
bool Boss::ActionSleep()
{
	bool endFlg = false;


	// 一定時間経過したら終了
	if (actionCount >= sleepFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	const int HEAL_TIMING =  actionCount % healFrame;

	// 回復のタイミングになったら回復
	if (HEAL_TIMING == 0)
	{
		if (health <= 5)health += 5;
		else if (health > 6)health = 10;
	}
	return endFlg;
}

// 追従状態
bool Boss::ActionFollow()
{
	bool endFlg = false;
	// 追従用にプレイヤーの位置を取ってくる
	XMFLOAT3 playerPos =  GameFunction::GetPlayerPos();

	// 追従前の待機時間よりも長いときに追従
	if(actionCount > followStopFrame)
	{
		const float SPEED = 0.6f;

		// 移動するベクトルの計算 (yはとりあえず固定値)
		XMVECTOR moveV = { playerPos.x - position.x,0,playerPos.z - position.z };
		moveV = DirectX::XMVector3Normalize(moveV);

		position.x += moveV.m128_f32[0] * SPEED;
		position.z += moveV.m128_f32[2] * SPEED;

	}

	// 一定時間経過したら終了
	if (actionCount >= followFrame)
	{
		endFlg = true;
		actionCount = 0;
	}
	else actionCount++;

	return endFlg;
}

// 突進状態
bool Boss::ActionRush()
{	
	bool endFlg = false;

	rushCount++;

	// 突進の待機中なら
	if (rushCount >= rushWaitFrame && !isRush)
	{
		isRush = true;
		rushCount = 0;

		// 追従用にプレイヤーの位置を取ってくる
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		rushV = { playerPos.x - position.x, 0, playerPos.z - position.z };
		rushV = DirectX::XMVector3Normalize(rushV);
	}

	if (isRush)
	{
		// 一回当たりの突進時間よりも経過時間が少なかったら
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


	// 一定時間経過したら終了
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

	// 攻撃関係初期化処理
	//if (initAttack)
	//{
	//	// 攻撃情報の生成
	//	attack = new AttackBase(1 * GAME_FRAME, 1 * GAME_FRAME, 10);
	//	// 完了したらfalseに
	//	initAttack = false;
	//}


	// 攻撃処理
	const int BLESS_TIMING = actionCount % blessWaitFrame;

	// blessのタイミングになったら攻撃
	if (BLESS_TIMING == 0)
	{
		// 攻撃方向の決定
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();
		XMVECTOR v = { playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z,0 };
		v = DirectX::XMVector3Normalize(v);
		// 方向の決定
		blessV.push_back(XMFLOAT3(v.m128_f32[0],v.m128_f32[1],v.m128_f32[2]));

		// 攻撃オブジェクトの生成
		Object* temp = new Object(NORMAL,"sphere");
		temp->SetPosition(position);
		temp->SetRadius(0.5f);

		attackObjs.push_back(temp);

	}
	
	actionCount++;

	// 攻撃の移動処理
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

	//　攻撃フレームを過ぎたら
	if (blessFrame <= actionCount)
	{
		endFlg = true;
		actionCount = 0;
	}

	// 終了条件を満たしていたらオブジェクトを消す
	if (endFlg)
	{
		// とりあえず終わったのがわかるようにパターンが終了したら弾を消す
		DestroyAttackArray();

		//blessV.clear();
		//blessV.shrink_to_fit();
	}

	return endFlg;
}


bool Boss::ActionFallConmbo()
{
	bool endFlg = false;

	// 攻撃関係初期化処理
	if (initAttack)
	{
		// easing用
		flyEasing.SetState(2, position.y, MAX_FLY_POS);

		startFly = true;
		// 完了したらfalseに
		initAttack = false;
		initEndFly = true;
	}
	
	// 開始時の処理
	if (startFly && !endFly)
	{
		position.y = flyEasing.StartEeaging(easeInFLAG);
		
		if (flyEasing.GetEndFlg())
		{
			startFly = false;
		}
	}
	else if(!startFly && !endFly)// 攻撃処理
	{
		// 家に帰ってから実装
		// 上昇した後振ってくる処理
		const float FLYATTACK_TIMING = actionCount % flyAttackFrame;
		if (FLYATTACK_TIMING == 0 && !flyAttackFlg)
		{
			flyAttackFlg = true;
			flyBack = false;

			// 落下方向の設定
			XMFLOAT3 playerPos = GameFunction::GetPlayerPos();
			flyComboV = { playerPos.x - position.x,playerPos.y - position.y,playerPos.z - position.z };
			flyComboV = DirectX::XMVector3Normalize(flyComboV);
			
		}

		if (flyAttackFlg)
		{
			if (!flyBack)
			{

				const float SPEED = 5;

				// 降下行動
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




	// 時間が経過したら終了フラグをtrue
	if (actionCount >= flyFrame && initEndFly && position.y >= MAX_FLY_POS)
	{
		endFly = true;
		flyEasing.SetState(2, position.y, 0);
		initEndFly = false;
	}

	// 終了時の着陸動作
	if (endFly)
	{
		position.y = flyEasing.StartEeaging(easeOutFLAG);
		
		if (flyEasing.GetEndFlg())
		{
			endFlg = true;
		}
	}

	// 終了条件に達していたら
	if (endFlg)
	{

		// 初期状態に戻す
		endFly = false;
		initEndFly = true;
		actionCount = 0;
	}

	return endFlg;
}

bool Boss::ActionFlyBless()
{
	bool endFlg = false;

	// 攻撃関係初期化処理
	if (initAttack)
	{


		// 完了したらfalseに
		initAttack = false;
	}

	return endFlg;
}

bool Boss::ActionLightning()
{
	bool endFlg = false;

	// 攻撃関係初期化処理
	if (initAttack)
	{


		// 完了したらfalseに
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
