#include "ObjectManager.h"
#include"Collision.h"
#include<string>
#include"GameFunction.h"
#include"Camera.h"

// �ÓI�����o�ϐ�
ObjectManager* ObjectManager::instance = nullptr;
std::vector<Object*> ObjectManager::objArray;
std::vector<Sprite2D*> ObjectManager::spriteArray;
//std::vector<std::string> ObjectManager::loadedFileArray;
Player* ObjectManager::player = nullptr;
Boss* ObjectManager::boss = nullptr;

// �֐�

void ObjectManager::CheckArray()
{
	// update�̍ŏ��Ŕz�����null���Ȃ����m�F������
// �����Ă����ꍇ�z�񂩂�폜���A�z����l�߂�
// obj�p�z�񐮗�
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
	// sprite�p
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
	// �z��폜
	// .obj�p�z��
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
	// sprite�p�z��
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

	// �v���C���[
	if (player != nullptr)
	{
		player->Destroy();
	}
	player = nullptr;

	// �G
	if (boss != nullptr)
	{
		boss->Destroy();
		delete boss;
	}
	boss = nullptr;

	// �C���X�^���X���폜
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
	// �z��`�F�b�N
	CheckArray();

	// �����蔻�菈�� �G������Ƃ�
	if (boss)
	{
		// player 2 boss
		Collision::Player2Enemy(*player, *boss,false);
		for (auto x : boss->GetEnemys())
		{
			Collision::Player2Enemy(*player, *x,true);
		}

		// �������U���̔���p
		std::vector<Object*>* bullet = boss->GetAttackObjPointer();
		const float BULLET_DAMAGE = boss->GetBlessDamage();
		if (bullet->size() > 0)
		{
			for (int i = 0; i < bullet->size(); i++)
			{
				// �e�ƃv���C���[������������I�u�W�F�N�g������
				if (Collision::Player2SphereOBJ(*player, *bullet->at(i),BULLET_DAMAGE))
				{
					delete bullet->at(i);
					bullet->at(i) = nullptr;
				}
			}

		}
	}


	// player�p
	if(player)player->Update();
	
	// enemy�p
	if (boss)
	{
		boss->Update();

		// �N���A�����p
		if (boss->GetHealth() <= 0)
		{
			// �Ƃ肠�����̃N���A�����B���p
			isClear = true;
		}
	}
	// �����܂�

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

// ��sprite

//void ObjectManager::AddSprite(int textureNum, const std::string& filename, XMFLOAT2 position, float rotation,XMFLOAT2 anchorPoint, XMFLOAT4 color)
//{
//	// �摜���[�h
//	Sprite2D::LoadTex(textureNum, filename);
//
//	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);
//
//	// �X�v���C�g����
//	temp->CreateSprite(textureNum);
//
//	// �ʒu�A�p�x�A�F�̐ݒ�
//	temp->SetPosition(position);
//	temp->SetRotation(rotation);
//	temp->SetColor(color);
//
//	// �z��ɒǉ�
//	spriteArray.push_back(temp);
//}

void ObjectManager::AddSprite(int textureNum,XMFLOAT2 position, float rotation, XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);

	temp->CreateSprite(textureNum);
	// �ʒu�A�p�x�A�F�̐ݒ�
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
