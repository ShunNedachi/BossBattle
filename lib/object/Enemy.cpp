#include "Enemy.h"
#include"GameFunction.h"

void Enemy::Init()
{
	// 仮用のオブジェクトの生成

	obj = new Object(NORMAL,"scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(2.5f);
}

void Enemy::Update()
{
	//	playerが必殺技中のとき動作を止める
	if (!GameFunction::GetPlayerIsSpecial())
	{
		const float SPEED = 0.1f;

		// 雑魚敵用行動　プレイヤーに追従
			// 追従用にプレイヤーの位置を取ってくる
		XMFLOAT3 playerPos = GameFunction::GetPlayerPos();

		// 移動するベクトルの計算 (yはとりあえず固定値)
		XMVECTOR moveV = { playerPos.x - position.x,0,playerPos.z - position.z };
		moveV = DirectX::XMVector3Normalize(moveV);

		position.x += moveV.m128_f32[0] * SPEED;
		position.z += moveV.m128_f32[2] * SPEED;



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
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->Update();
}

void Enemy::Draw()
{
	obj->Draw();
}

void Enemy::Destroy()
{
	// 仮用の処理
	delete obj;
	obj = nullptr;
}

void Enemy::RecieveDamage(XMFLOAT3 pos, float damage,bool knockback)
{
	// 
	isDamage = true;
	health -= damage;

	// ダメージを受けた時に位置をずらす
	XMVECTOR movePos{};
	// 移動用　とりあえずxzのみ移動
	movePos.m128_f32[0] = (pos.x - position.x);
	movePos.m128_f32[2] = (pos.z - position.z);
	

	movePos = DirectX::XMVector3Normalize(movePos);

	// ノックバック処理
	if (knockback)
	{
		position.x -= movePos.m128_f32[0] * 10;
		position.z -= movePos.m128_f32[2] * 10;

		obj->SetPosition(position);
	}
}
