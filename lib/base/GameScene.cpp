#include "GameScene.h"

GameScene::~GameScene()
{
	objectManager->Destroy();

}

void GameScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();

	// ���\�[�X���[�h
	objectManager->AddOBJ("triangle", { 0,0,-100 });

	camera = Camera::GetInstance();

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


	objectManager->Update();
	camera->UpdateFollow(objectManager->GetPlayerPos());


	if (input->TriggerKey(DIK_1))NextScene(SceneManager::GetInstance());
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


