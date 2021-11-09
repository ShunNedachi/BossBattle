#include "ObjectManager.h"
#include"Collision.h"
#include<string>

// �ÓI�����o�ϐ�
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
	if (boss)
	{
		boss->Update();

		if (boss->GetHealth() <= 0)
		{
			boss->Destroy();
			delete boss;
			boss = nullptr;

			// �Ƃ肠�����̃N���A�����B���p
			isClear = true;
		}
	}

	// �����蔻�菈�� �G������Ƃ�
	if (boss)
	{
		// �����蔻��p�@�萔�l
		// �ʒu���̊l��
		const XMFLOAT3 PLAYER_POS = player->GetPos();
		const XMFLOAT3 BOSS_POS = boss->GetPosition();
		const AttackBase* PLAYER_ATTACK = player->GetAttack();
		const XMFLOAT3 PLAYER_ATTACK_POS = player->GetAttackPos();
		const Object* BOSS_OBJ = boss->GetOBJ();
		const std::vector<Enemy*> ENEMYS = boss->GetEnemys();
		// �T�C�Y�p
		const float PLAYER_RADIUS = player->GetRadius();
		const float BOSS_RADIUS = boss->GetRadius();

		// �_���[�W�@��ԁ@�ʂ̎擾
		const bool BOSS_ISDAMAGE = boss->GetIsDamage();
		const bool PLAYER_ISDAMAGE = player->GetIsDamage();
		const int PLAYER_DAMAGE = player->GetDamage();
		const int BOSS_DAMAGE = boss->GetDamage();

		// �v���C���[�̍U����boss�Ƃ̔���
		if (Collision::Attack2OBJ(*BOSS_OBJ, *PLAYER_ATTACK))
		{
			if (!BOSS_ISDAMAGE)boss->RecieveDamage(PLAYER_ATTACK_POS, PLAYER_DAMAGE);
		}
		// �v���C���[��boss�Ƃ̔���
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

			// �v���C���[�̍U����enemys�̔���
			if (Collision::Attack2OBJ(*ENEMY_OBJ, *PLAYER_ATTACK))
			{
				if (!ENEMY_ISDAMAGE)x->RecieveDamage(PLAYER_ATTACK_POS, PLAYER_DAMAGE);
			}

			// �v���C���[��enemys�̔���
			if (Collision::Sphere2Sphere(PLAYER_POS, ENEMY_POS, PLAYER_RADIUS, ENEMY_RADIUS))
			{
				if (!PLAYER_ISDAMAGE)player->RecieveDamage(ENEMY_POS, ENEMY_DAMAGE);
			}
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

void ObjectManager::AddSprite(int textureNum,XMFLOAT2 position, float rotation, XMFLOAT2 anchorPoint, XMFLOAT4 color)
{
	Sprite2D* temp = new Sprite2D(0.5f, 0.5f);

	temp->CreateSprite(textureNum);
	// �ʒu�A�p�x�A�F�̐ݒ�
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