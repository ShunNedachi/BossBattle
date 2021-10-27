#include "ObjectManager.h"
#include"SpriteLoadDefine.h"
#include"Collision.h"
#include<string>

// 静的メンバ変数
ObjectManager* ObjectManager::instance = nullptr;
std::vector<Object*> ObjectManager::objArray;
std::vector<Sprite2D*> ObjectManager::spriteArray;
std::vector<std::string> ObjectManager::loadedFileArray;
Player* ObjectManager::player = nullptr;
Enemy* ObjectManager::enemy = nullptr;

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
	if (enemy != nullptr)
	{
		enemy->Destroy();
		delete enemy;
	}
	enemy = nullptr;

	if(instance != nullptr)delete instance;
	instance = nullptr;
}

void ObjectManager::Initialize(MyDirectX12* my12, MyWindow* window)
{
	Object::Init(my12, window);
	Sprite2D::Init(my12, window);
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
	if (enemy)
	{
		enemy->Update();
		if (enemy->GetHealth() <= 0)
		{
			enemy->Destroy();
			delete enemy;
			enemy = nullptr;
		}
	}

	// 当たり判定処理
	if (enemy)
	{
		// プレイヤーの攻撃と敵との判定
		if (Collision::Attack2OBJ(enemy->GetOBJ(), player->GetAttack()))
		{
			if (!enemy->GetIsDamage())enemy->RecieveDamage(player->GetAttackPos(), player->GetDamage());
		}

		// プレイヤーと敵との判定
		if (Collision::Sphere2Sphere(player->GetPos(), enemy->GetPosition(), player->GetRadius(), enemy->GetRadius()))
		{
			if(!player->GetIsDamage())player->RecieveDamage(enemy->GetPosition(), enemy->GetDamage());
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

	if (enemy)enemy->Draw();

	for (auto x : spriteArray)
	{
		if(x != nullptr)x->Draw();
	}
}

void ObjectManager::AddPlayer(const std::string& filename)
{
	ObjectManager::player = player->GetInstance();

	// プレイヤーのスプライト関係のロード
	Sprite2D::LoadTex(playerHealthBar, TEXT("playerHealth.png"));
	Sprite2D::LoadTex(playerHealth, TEXT("playerHealth1Dot.png"));

	ObjectManager::player->Init(filename);
}

void ObjectManager::AddEnemy()
{
	enemy = new Enemy();
	enemy->Init();
	enemy->SetRadius(1);
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

void ObjectManager::DeleteSprite(int index)
{
	if (spriteArray.size() > 0 && spriteArray.size() <= index)
	{
		delete spriteArray[index];
		spriteArray[index] = nullptr;
	}
}