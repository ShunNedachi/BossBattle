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
	
	
	objectManager->Update();
	camera->Update();

	// �J�����̋���
	if (objectManager->GetIsInitBoss())
	{
		// player�ɒǏ]
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		const float R = 10;
		const float Phi = 300;
		const float Theta = 20;

		// �G�̓o��^�C�~���O�œG�ɃJ������Ǐ]
		camera->SetState(R, Theta, Phi);
		camera->Follow(objectManager->GetBossPosition());
	}
	

	//// �V�[���ύX
	if (objectManager->GetPlayerDead())ChangeScene(SceneManager::GetInstance(),endSceneNum);
	else if (objectManager->GetIsClear())ChangeScene(SceneManager::GetInstance(),clearSceneNum);

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


