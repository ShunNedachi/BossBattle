#include "Boss.h"
#include"GameFunction.h"

void Boss::Init()
{
	// 仮用のオブジェクトの生成

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
	// enemyの中身にnullがあるかの確認
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


	if (!GameFunction::GetPlayerIsSpecial())
	{
		Action();

		// ダメージ状態時の処理
		if (isDamage)
		{
			damageCount++;
			obj->SetColor({ 0,1,1 });

			// ダメージ状態解除条件
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

	default:
		break;
	}

	// 行動変更時　条件によってパターン変更
	if (result)
	{
		// debug用に一旦コメント

		//if (pattern != BossPattern::Bless)pattern = (BossPattern)((int)pattern + 1);
		//else pattern = BossPattern::Stop;
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

			tempPos.x += blessV[i].x;
			tempPos.y += blessV[i].y;
			tempPos.z += blessV[i].z;

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

bool Boss::ActionFallConmbo()
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
