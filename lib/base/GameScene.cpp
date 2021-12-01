#include "GameScene.h"
#include"ObjectManager.h"
#include"GameFunction.h"

GameScene::~GameScene()
{
	objectManager->Destroy();

}

void GameScene::Initalize()
{
	camera = Camera::GetInstance();
	// ‰Šú‰æŠp—p
	Camera::SetTheta(20);

	GameFunction::LoadGameSceneTexture();

	objectManager = ObjectManager::GetInstance();

	// Šî–{obj‚ÌÝ’u
	objectManager->AddPlayer("sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 });
	objectManager->AddOBJ("skydome");
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddEnemy();
}

void GameScene::Update()
{
	// inputŠÖŒW
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// ƒJƒƒ‰‚Ì‹““®
	if (objectManager->GetIsInitBoss())
	{
		// player‚É’Ç]
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		// “G‚Ì“oêƒ^ƒCƒ~ƒ“ƒO‚Å“G‚ÉƒJƒƒ‰‚ð’Ç]
		camera->Follow(objectManager->GetBossPosition());
		camera->SetPhi(300);
		camera->SetTheta(20);
		camera->SetR(5);
	}

	camera->Update();


	objectManager->Update();

	//// ƒV[ƒ“•ÏX
	if (objectManager->GetPlayerDead())NextScene(SceneManager::GetInstance());
	else if (objectManager->GetIsClear())NextScene(SceneManager::GetInstance());

}

void GameScene::Draw()
{
	// ƒQ[ƒ€ƒIƒuƒWƒFƒNƒg‚Ì•`‰æ
	objectManager->Draw();
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


