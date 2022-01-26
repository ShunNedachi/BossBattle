#include "ObjectManager.h"
#include"Collision.h"
#include<string>
#include"GameFunction.h"
#include"Camera.h"

// 静的メンバ変数
ObjectManager* ObjectManager::instance = nullptr;
std::vector<Object*> ObjectManager::objArray;
std::vector<Sprite2D*> ObjectManager::spriteArray;
//std::vector<std::string> ObjectManager::loadedFileArray;
Player* ObjectManager::player = nullptr;
Boss* ObjectManager::boss = nullptr;

// 関数

void ObjectManager::CheckArray()
{
	// updateの最初で配列内にnullがないか確認をする
// 入っていた場合配列から削除し、配列を詰める
// obj用配列整理
	for (auto itr = objArray.begin(); itr < objArray.end();)
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
}

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
	// 配列削除
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

	// インスタンスを削除
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
	// 配列チェック
	CheckArray();

	// 当たり判定処理 敵がいるとき
	if (boss)
	{
		// player 2 boss
		Collision::Player2Enemy(*player, *boss,false);
		for (auto x : boss->GetEnemys())
		{
			Collision::Player2Enemy(*player, *x,true);
		}

		// 遠距離攻撃の判定用
		std::vector<Object*>* bullet = boss->GetAttackObjPointer();
		const float BULLET_DAMAGE = boss->GetBlessDamage();
		if (bullet->size() > 0)
		{
			for (int i = 0; i < bullet->size(); i++)
			{
				// 弾とプレイヤーが当たったらオブジェクトを消す
				if (Collision::Player2SphereOBJ(*player, *bullet->at(i),BULLET_DAMAGE))
				{
					delete bullet->at(i);
					bullet->at(i) = nullptr;
				}
			}

		}
	}


	// player用
	if(player)player->Update();
	
	// enemy用
	if (boss)
	{
		boss->Update();

		// クリア条件用
		if (boss->GetHealth() <= 0)
		{
			// とりあえずのクリア条件達成用
			isClear = true;
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

void ObjectManager::AddBoss()
{
	boss = new Boss();
	boss->Init();
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

//void ObjectManager::AddSprite(int textureNum, const std::string& filename, XMFLOAT2 position, float rotation,XMFLOAT2 anchorPoint, XMFLOAT4 color)
//{
//	// 画像ロード
//	Sprite2D::LoadTex(textureNum, filename);
//
//	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);
//
//	// スプライト生成
//	temp->CreateSprite(textureNum);
//
//	// 位置、角度、色の設定
//	temp->SetPosition(position);
//	temp->SetRotation(rotation);
//	temp->SetColor(color);
//
//	// 配列に追加
//	spriteArray.push_back(temp);
//}

void ObjectManager::AddSprite(int textureNum,XMFLOAT2 position, float rotation, XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);

	temp->CreateSprite(textureNum);
	// 位置、角度、色の設定
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetColor(color);
	
	spriteArray.push_back(temp);
}

void ObjectManager::DeleteSprite(int index)
{
	if (spriteArray.size() >= index)
	{
		delete spriteArray[index];
		spriteArray[index] = nullptr;
	}
}

void ObjectManager::SpriteFlash(int index, bool flg)
{
	if (spriteArray.size() >= index)
	{
		spriteArray[index]->SetDrawFlash(flg);
	}
}

void ObjectManager::SetSize(int index, XMFLOAT2 size)
{
	if (spriteArray.size() >= index)
	{
		spriteArray[index]->Resize(size.x, size.y);
	}
}

void ObjectManager::SetFlashSpeed(int index, float speed)
{
	if (spriteArray.size() >= index)
	{
		spriteArray[index]->SetFlashSpeed(speed);
	}
}

DirectX::XMFLOAT4 ObjectManager::GetColor(int index)
{
	if (spriteArray.size() >= index)
	{
		return spriteArray[index]->GetColor();
	}

	return { -200,-200,-200,-200 };
}
