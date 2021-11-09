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
	obj->SetRadius(10);
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


	obj->Update();

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Update();
	}

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

	// 後でaiに引っ越し
	if (Xinput::GetInstance()->TriggerButtom(0,xinput_X))
	{
		AddEnemy({ 30,0,0 }, {10,10,10});
	}
}

void Boss::Draw()
{
	obj->Draw();

	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Draw();
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

	enemys.push_back(temp);
}
