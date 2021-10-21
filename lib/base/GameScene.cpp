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

	objectManager->AddPlayer("player");
	objectManager->AddOBJ("ground", { 0,-1,0 });
	objectManager->AddOBJ("skydome");
	camera->SetEye({ 0, 20, -100 });

	//camera->SetTarget(objectManager->GetPlayerPos());
}

void GameScene::Update()
{
	// input�֌W
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	//camera->Update();
	//camera->SetTarget(objectManager->GetPlayerPos());
	//camera->FollowTarget();
	camera->UpdateFollow(objectManager->GetPlayerPos());

	objectManager->Update();


	//DirectX::XMFLOAT3 tempPos = objectManager->GetPlayerPos();
	//camera->SetEye({ tempPos.x,tempPos.y,tempPos.z - 10 });

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


