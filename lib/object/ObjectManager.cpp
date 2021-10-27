#include "ObjectManager.h"
#include"SpriteLoadDefine.h"
#include"Collision.h"
#include<string>

// �ÓI�����o�ϐ�
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
	// �C���X�^���X���폜
	

	// �z����폜
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
	// update�̍ŏ��Ŕz�����null���Ȃ����m�F������
	// �����Ă����ꍇ�z�񂩂�폜���A�z����l�߂�
	// obj�p�z�񐮗�
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



	// player�p
	if(player)player->Update();
	
	// enemy�p
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

	// �����蔻�菈��
	if (enemy)
	{
		// �v���C���[�̍U���ƓG�Ƃ̔���
		if (Collision::Attack2OBJ(enemy->GetOBJ(), player->GetAttack()))
		{
			if (!enemy->GetIsDamage())enemy->RecieveDamage(player->GetAttackPos(), player->GetDamage());
		}

		// �v���C���[�ƓG�Ƃ̔���
		if (Collision::Sphere2Sphere(player->GetPos(), enemy->GetPosition(), player->GetRadius(), enemy->GetRadius()))
		{
			if(!player->GetIsDamage())player->RecieveDamage(enemy->GetPosition(), enemy->GetDamage());
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

	if (enemy)enemy->Draw();

	for (auto x : spriteArray)
	{
		if(x != nullptr)x->Draw();
	}
}

void ObjectManager::AddPlayer(const std::string& filename)
{
	ObjectManager::player = player->GetInstance();

	// �v���C���[�̃X�v���C�g�֌W�̃��[�h
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

// ��sprite

void ObjectManager::AddSprite(int textureNum, const std::string& filename, XMFLOAT2 position, float rotation,XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	// �摜���[�h
	Sprite2D::LoadTex(textureNum, filename);

	Sprite2D* temp = new Sprite2D(anchorPoint.x, anchorPoint.y);

	// �X�v���C�g����
	temp->CreateSprite(textureNum);

	// �ʒu�A�p�x�A�F�̐ݒ�
	temp->SetPosition(position);
	temp->SetRotation(rotation);
	temp->SetColor(color);

	// �z��ɒǉ�
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