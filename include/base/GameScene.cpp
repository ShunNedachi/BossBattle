#include "GameScene.h"
#include"ObjectManager.h"
#include"GameFunction.h"
#include"Collision.h"

GameScene::~GameScene()
{
	objectManager->Destroy();
}

void GameScene::Initalize()
{
	camera = Camera::GetInstance();
	// ������p�p
	Camera::SetTheta(20);

	GameFunction::LoadGameSceneTexture();

	objectManager = ObjectManager::GetInstance();

	// ��{obj�̐ݒu
	objectManager->AddPlayer("sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 }, { 10,10,10 });
	objectManager->AddOBJ("skydome", { 0,0,0 }, { 5,5,5 });
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddBoss();
}

void GameScene::Update()
{
	// input�֌W
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	// �I�u�W�F�N�g�̃f�[�^�������Ă���
	Player* player = objectManager->GetPlayer();
	Boss* boss = objectManager->GetBoss();
	Light* light = objectManager->GetLight();
	std::vector<Object*> objects = *objectManager->GetObjectArray();

	#pragma region �X�V����

	light->Update();
	player->Update();
	boss->Update();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}
	camera->Update();
#pragma endregion


	// �J�����̋���
	if (boss->GetIsInit())
	{
		// player�ɒǏ]
		camera->Follow(player->GetPos());


		// �{�̓��m�̓����蔻��
		Collision::Player2Enemy(*player, *boss, false);
		for (auto x : boss->GetEnemys())
		{
			Collision::Player2Enemy(*player, *x, true);
		}

		// �������U���p����
		std::vector<Object*> bullet = *boss->GetAttackObjPointer();
		for (int i = 0; i < bullet.size(); i++)
		{
			// �e�Ƀv���C���[���������Ă����Ƃ��ɃI�u�W�F�N�g�̍폜
			if (Collision::Player2SphereOBJ(*player, *bullet[i], boss->GetBlessDamage()))
			{
				delete bullet[i];
				bullet[i] = nullptr;
			}
		}

	}
	else
	{
		const float R = 10;
		const float Phi = 300;
		const float Theta = 20;

		// �G�̓o��^�C�~���O�œG�ɃJ������Ǐ]
		camera->SetState(R, Theta, Phi);
		camera->Follow(boss->GetPosition());
	}
	

	//// �V�[���ύX
	if (player->GetIsDead())ChangeScene(SceneManager::GetInstance(),endSceneNum);
	else if (boss->GetHealth() <= 0)ChangeScene(SceneManager::GetInstance(),clearSceneNum);

}

void GameScene::Draw()
{
	// �Q�[���I�u�W�F�N�g�̕`��
	Player* player = objectManager->GetPlayer();
	Boss* boss = objectManager->GetBoss();
	Light* light = objectManager->GetLight();

	std::vector<Object*> objectArray = *objectManager->GetObjectArray();
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();

	for (int i = 0; i < spriteArray.size(); i++)
	{
		spriteArray[i]->Draw();
	}

	for (int i = 0; i < objectArray.size(); i++)
	{
		objectArray[i]->Draw(*light);
	}
	player->Draw(*light);
	boss->Draw(*light);



	//objectManager->Draw();
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


