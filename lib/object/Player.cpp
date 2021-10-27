#include "Player.h"
#include"SpriteLoadDefine.h"

// 静的変数の実体
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

	// 体力
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
	// プレイヤーのオブジェクト用
	objPlayer = new Object(NORMAL,filename);

	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };

	objPlayer->SetScale(scale);
	objPlayer->SetRadius(1);
	// ここまで

	// 体力用
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
	// ここまで

	// 攻撃関係

	attack = new AttackBase(0, 10, 10);
	attackObj = new Object(NORMAL,"sphere");
	attackObj->SetScale({ 2,2,2 });
	attackObj->SetRadius(2);

	attack->SetObject(attackObj);
	// ここまで
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	
	const bool XINPUT_TRIGGER_A = xinput->TriggerButtom(0, xinput_A);

	// 移動処理　後で関数化して処理を分かりやすく
	const int XINPUT_MOVE_STICK = xinput->MoveStick(0, xinput_LS);

	// 入力確認
	if (XINPUT_MOVE_STICK & XINPUT_STICK_UP ||
		XINPUT_MOVE_STICK & XINPUT_STICK_DOWN||
		XINPUT_MOVE_STICK & XINPUT_STICK_LEFT ||
		XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
	{
		//	攻撃中じゃないときに移動可能
		if (!attack->GetAttackStart())
		{
			move = {0,0,0};
			XMFLOAT3 attackPos = position;
			bool isMoveRIGHTLEFT = false;

			// 外積に必要な変数の定義
			XMFLOAT3 eyeDir = Camera::GetEyeDir();
			XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
			XMVECTOR upV = { Camera::GetUp().x,Camera::GetUp().y,Camera::GetUp().z,0 };
			// プレイヤーの位置からupベクトルをとる
			XMVECTOR playerUpV = {position.x,position.y,position.z};
			playerUpV.m128_f32[1] += 1;
			// eyeDirV と　upの外積
			// 左右移動用
			XMVECTOR moveRIGHTLEFT = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
			// 上下移動用
			XMVECTOR moveUPDOWN = DirectX::XMVector3Normalize(DirectX::XMVector3Cross( moveRIGHTLEFT, playerUpV));

			// 直接posを変更して、とりあえず視線方向に移動
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

			// 上下移動しているのなら
			if (isMoveRIGHTLEFT)
			{
				move = DirectX::XMVector3Normalize(move);

				position.x += move.m128_f32[0];
				position.z += move.m128_f32[2];

				attackPos.x += move.m128_f32[0] * 5;
				attackPos.z += move.m128_f32[2] * 5;
				// 左右移動用に初期化
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
	// ここまで

	//// 回転設定用 　yに-90しているのはモデルの初期値に対するものあとで変更
	//// 攻撃中は回転を止める
	if(!attack->GetAttackStart())attackObj->SetRotation({ 0, -1 *Camera::GetPhi()- 90,0 });




	// 攻撃用処理
	
	if (XINPUT_TRIGGER_A)
	{
		// 攻撃用見た目オブジェクトの位置変更
		XMFLOAT3 eyeDir = Camera::GetEyeDir();
		// 距離変更用
		const float DISTANCE = 5;

		//XMFLOAT3 tempPos = { position.x + move.m128_f32[0] * DISTANCE,position.y,position.z + move.m128_f32[2] * DISTANCE};
		//attackObj->SetPosition(tempPos);

		// 正面方向に向きを合わせる(後程)

		attack->SetAttackStart();
	}

	// ここまで


	// ダメージ状態時の処理
	if (isDamage)
	{
		damageCount++;
		objPlayer->SetColor({ 0,1,1 });

		// ダメージ状態解除条件
		if (damageCount >= DAMAGE_FRAME)
		{
			isDamage = false;
			damageCount = 0;
			objPlayer->SetColor({ 1,1,1 });
		}

	}


	//	更新

	objPlayer->Update();

	attackObj->Update();
	attack->Update();
}

void Player::Draw()
{
	// 後でダメージ状態の時に赤く点滅するように修正する
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

	// ダメージを受けた時に位置をずらす
	XMVECTOR movePos;
	// 移動用　とりあえずxzのみ移動
	movePos.m128_f32[0] = (pos.x - position.x);
	movePos.m128_f32[2] = (pos.z - position.z);

	//movePos = DirectX::XMVector4Normalize(movePos);

	// ノックバック処理
	position.x -= movePos.m128_f32[0]* 10;
	position.z -= movePos.m128_f32[2]* 10;

	objPlayer->SetPosition(position);
}
