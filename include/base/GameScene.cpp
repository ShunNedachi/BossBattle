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
	// 初期画角用
	Camera::SetTheta(20);

	GameFunction::LoadGameSceneTexture();

	objectManager = ObjectManager::GetInstance();

	// 基本objの設置
	objectManager->AddPlayer("sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 }, { 10,10,10 });
	objectManager->AddOBJ("skydome", { 0,0,0 }, { 5,5,5 });
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddBoss();
}

void GameScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	// オブジェクトのデータを持ってくる
	Player* player = objectManager->GetPlayer();
	Boss* boss = objectManager->GetBoss();
	Light* light = objectManager->GetLight();
	std::vector<Object*> objects = *objectManager->GetObjectArray();

	#pragma region 更新処理

	light->Update();
	player->Update();
	boss->Update();
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}
	camera->Update();
#pragma endregion


	// カメラの挙動
	if (boss->GetIsInit())
	{
		// playerに追従
		camera->Follow(player->GetPos());


		// 本体同士の当たり判定
		Collision::Player2Enemy(*player, *boss, false);
		for (auto x : boss->GetEnemys())
		{
			Collision::Player2Enemy(*player, *x, true);
		}

		// 遠距離攻撃用判定
		std::vector<Object*> bullet = *boss->GetAttackObjPointer();
		for (int i = 0; i < bullet.size(); i++)
		{
			// 弾にプレイヤーが当たっていたときにオブジェクトの削除
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

		// 敵の登場タイミングで敵にカメラを追従
		camera->SetState(R, Theta, Phi);
		camera->Follow(boss->GetPosition());
	}
	

	//// シーン変更
	if (player->GetIsDead())ChangeScene(SceneManager::GetInstance(),endSceneNum);
	else if (boss->GetHealth() <= 0)ChangeScene(SceneManager::GetInstance(),clearSceneNum);

}

void GameScene::Draw()
{
	// ゲームオブジェクトの描画
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


