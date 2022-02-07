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
	objectManager->AddOBJ("ground", { 0,-1,0 }, { 10,10,10 });
	objectManager->AddOBJ("skydome", { 0,0,0 }, { 5,5,5 });
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddBoss();
}

void GameScene::Update()
{
	// inputŠÖŒW
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	
	objectManager->Update();
	camera->Update();

	// ƒJƒƒ‰‚Ì‹““®
	if (objectManager->GetIsInitBoss())
	{
		// player‚É’Ç]
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		const float R = 10;
		const float Phi = 300;
		const float Theta = 20;

		// “G‚Ì“oêƒ^ƒCƒ~ƒ“ƒO‚Å“G‚ÉƒJƒƒ‰‚ð’Ç]
		camera->SetState(R, Theta, Phi);
		camera->Follow(objectManager->GetBossPosition());
	}
	

	//// ƒV[ƒ“•ÏX
	if (objectManager->GetPlayerDead())ChangeScene(SceneManager::GetInstance(),endSceneNum);
	else if (objectManager->GetIsClear())ChangeScene(SceneManager::GetInstance(),clearSceneNum);

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


