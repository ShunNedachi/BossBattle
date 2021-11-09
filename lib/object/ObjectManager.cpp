#include "ObjectManager.h"
#include"Collision.h"
#include<string>

// 静的メンバ変数
ObjectManager* ObjectManager::instance = nullptr;
std::vector<Object*> ObjectManager::objArray;
std::vector<Sprite2D*> ObjectManager::spriteArray;
std::vector<std::string> ObjectManager::loadedFileArray;
Player* ObjectManager::player = nullptr;
Boss* ObjectManager::boss = nullptr;

ObjectManager* ObjectManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ObjectManager();
	}

	return instance;
}

void ObjectManager::Destroy()
{
	// インスタンスを削除
	

	// 配列も削除
	// .obj用配列
	if (objArray.size() > 0)
	{
		for (auto& x : objArray)
		{
			delete x;
			x = nullptr;
		}

		objArray.clear();
		objArray.shrink_to_fit();
	}
	// sprite用配列
	if (spriteArray.size() > 0)
	{
		for (auto& x : spriteArray)
		{
			delete x;
			x = nullptr;
		}

		spriteArray.clear();
		spriteArray.shrink_to_fit();
	}

	// プレイヤー
	if (player != nullptr)
	{
		player->Destroy();
	}
	player = nullptr;

	// 敵
	if (boss != nullptr)
	{
		boss->Destroy();
		delete boss;
	}
	boss = nullptr;

	if(instance != nullptr)delete instance;
	instance = nullptr;
}

void ObjectManager::Initialize(MyDirectX12* my12)
{
	Object::Init(my12);
	Sprite2D::Init(my12);
}

void ObjectManager::Update()
{
	// updateの最初で配列内にnullがないか確認をする
	// 入っていた場合配列から削除し、配列を詰める
	// obj用配列整理
	for (auto itr  = objArray.begin();itr < objArray.end();)
	{
		if (*itr == nullptr)
		{
			itr = objArray.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	// sprite用
	for (auto itr = spriteArray.begin(); itr < spriteArray.end();)
	{
		if (*itr == nullptr)
		{
			itr = spriteArray.erase(itr);
		}
		else
		{
			itr++;
		}
	}



	// player用
	if(player)player->Update();
	
	// enemy用
	if (boss)
	{
		boss->Update();

		if (boss->GetHealth() <= 0)
		{
			boss->Destroy();
			delete boss;
			boss = nullptr;

			// とりあえずのクリア条件達成用
			isClear = true;
		}
	}

	// 当たり判定処理 敵がいるとき
	if (boss)
	{
		// 当たり判定用　定数値
		// 位置情報の獲得
		const XMFLOAT3 PLAYER_POS = player->GetPos();
		const XMFLOAT3 BOSS_POS = boss->GetPosition();
		const AttackBase* PLAYER_ATTACK = player->GetAttack();
		const XMFLOAT3 PLAYER_ATTACK_POS = player->GetAttackPos();
		const Object* BOSS_OBJ = boss->GetOBJ();
		const std::vector<Enemy*> ENEMYS = boss->GetEnemys();
		// サイズ用
		const float PLAYER_RADIUS = player->GetRadius();
		const float BOSS_RADIUS = boss->GetRadius();

		// ダメージ　状態　量の取得
		const bool BOSS_ISDAMAGE = boss->GetIsDamage();
		const bool PLAYER_ISDAMAGE = player->GetIsDamage();
		const int PLAYER_DAMAGE = player->GetDamage();
		const int BOSS_DAMAGE = boss->GetDamage();

		// プレイヤーの攻撃とbossとの判定
		if (Collision::Attack2OBJ(*BOSS_OBJ, *PLAYER_ATTACK))
		{
			if (!BOSS_ISDAMAGE)boss->RecieveDamage(PLAYER_ATTACK_POS, PLAYER_DAMAGE);
		}
		// プレイヤーとbossとの判定
		if (Collision::Sphere2Sphere(PLAYER_POS, BOSS_POS,PLAYER_RADIUS, BOSS_RADIUS))
		{
			if(!PLAYER_ISDAMAGE)player->RecieveDamage(BOSS_POS, BOSS_DAMAGE);
		}

		for (auto x : ENEMYS)
		{
			const Object* ENEMY_OBJ = x->GetOBJ();
			const XMFLOAT3 ENEMY_POS = x->GetPosition();
			const bool ENEMY_ISDAMAGE = x->GetIsDamage();
			const float ENEMY_RADIUS = x->GetRadius();
			const int ENEMY_DAMAGE = x->GetDamage();

			// プレイヤーの攻撃とenemysの判定
			if (Collision::Attack2OBJ(*ENEMY_OBJ, *PLAYER_ATTACK))
			{
				if (!ENEMY_ISDAMAGE)x->RecieveDamage(PLAYER_ATTACK_POS, PLAYER_DAMAGE);
			}

			// プレイヤーとenemysの判定
			if (Collision::Sphere2Sphere(PLAYER_POS, ENEMY_POS, PLAYER_RADIUS, ENEMY_RADIUS))
			{
				if (!PLAYER_ISDAMAGE)player->RecieveDamage(ENEMY_POS, ENEMY_DAMAGE);
			}
		}
	}

	// ここまで

}

void ObjectManager::Draw()
{
	for (auto x : objArray)
	{
		if(x != nullptr)x->Draw();
	}

	if(player)player->Draw();

	if (boss)boss->Draw();

	for (auto x : spriteArray)
	{
		if(x != nullptr)x->Draw();
	}
}

void ObjectManager::AddPlayer(const std::string& filename)
{
	ObjectManager::player = player->GetInstance();

	ObjectManager::player->Init(filename);
}

void ObjectManager::AddEnemy()
{
	boss = new Boss();
	boss->Init();
	boss->SetRadius(1);
}

void ObjectManager::AddOBJ(const std::string& filename,XMFLOAT3 position,XMFLOAT3 scale,XMFLOAT3 rotation,int drawShader)
{
	Object* obj = new Object(drawShader,filename);

	obj->SetPosition(position);
	obj->SetRotation(rotation);
	obj->SetScale(scale);

	obj->Update();

	objArray.push_back(obj);
}

void ObjectManager::DeleteOBJ(int index)
{
	if (objArray.size() > 0 && objArray.size() <= index) 
	{
		delete objArray[index];
		objArray[index] = nullptr;
	}
}

// ↓sprite

void ObjectManager::AddSprite(int textureNum, const std::string& filename, XMFLOAT2 position, float rotation,XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	// 画像ロード
	Sprite2D::LoadTex(textureNum, filename);

	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);

	// スプライト生成
	temp->CreateSprite(textureNum);

	// 位置、角度、色の設定
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetColor(color);

	// 配列に追加
	spriteArray.push_back(temp);
}

void ObjectManager::AddSprite(int textureNum,XMFLOAT2 position, float rotation, XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	Sprite2D* temp = new Sprite2D(0.5f, 0.5f);

	temp->CreateSprite(textureNum);
	// 位置、角度、色の設定
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetColor(color);
	
	spriteArray.push_back(temp);
}

void ObjectManager::DeleteSprite(int index)
{
	if (spriteArray.size() > 0 && spriteArray.size() <= index)
	{
		delete spriteArray[index];
		spriteArray[index] = nullptr;
	}
}