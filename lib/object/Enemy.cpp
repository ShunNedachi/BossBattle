#include "Enemy.h"

void Enemy::Init()
{
	// 仮用のオブジェクトの生成

	obj = new Object(NORMAL,"scaffold");

	position = { 0,0,20 };
	scale = { 5,5,5 };
	obj->SetPosition(position);
	obj->SetScale(scale);
	obj->SetRadius(5);
}

void Enemy::Update()
{

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

void Enemy::RecieveDamage(XMFLOAT3 pos, float damage)
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
	position.x -= movePos.m128_f32[0] * 10;
	position.z -= movePos.m128_f32[2] * 10;

	obj->SetPosition(position);

}
