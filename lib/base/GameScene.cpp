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
	// ������p�p
	Camera::SetTheta(20);

	GameFunction::LoadGameSceneTexture();

	objectManager = ObjectManager::GetInstance();

	// ��{obj�̐ݒu
	objectManager->AddPlayer("sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 });
	objectManager->AddOBJ("skydome");
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddEnemy();
}

void GameScene::Update()
{
	// input�֌W
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// �J�����̋���
	if (objectManager->GetIsInitBoss())
	{
		// player�ɒǏ]
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		// �G�̓o��^�C�~���O�œG�ɃJ������Ǐ]
		camera->Follow(objectManager->GetBossPosition());
		camera->SetPhi(300);
		camera->SetTheta(20);
		camera->SetR(5);
	}

	camera->Update();


	objectManager->Update();

	//// �V�[���ύX
	if (objectManager->GetPlayerDead())NextScene(SceneManager::GetInstance());
	else if (objectManager->GetIsClear())NextScene(SceneManager::GetInstance());

}

void GameScene::Draw()
{
	// �Q�[���I�u�W�F�N�g�̕`��
	objectManager->Draw();
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


