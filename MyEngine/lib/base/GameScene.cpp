#include "GameScene.h"

GameScene::~GameScene()
{
	objectManager->Destroy();

}

void GameScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();

	// リソースロード
	objectManager->AddOBJ("triangle", { 0,0,-100 });

	camera = Camera::GetInstance();

	objectManager->AddPlayer("Sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 });
	objectManager->AddOBJ("skydome");
	camera->SetEye({ 0, 20, -100 });
}

void GameScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	//camera->Update();
	//camera->FollowTarget();
	camera->SetTarget(objectManager->GetPlayerPos());
	camera->UpdateRot();

	objectManager->Update();


	//DirectX::XMFLOAT3 tempPos = objectManager->GetPlayerPos();
	//camera->SetEye({ tempPos.x,tempPos.y,tempPos.z - 10 });

	if (input->TriggerKey(DIK_1))NextScene(SceneManager::GetInstance());
}

void GameScene::Draw()
{
	// ゲームオブジェクトの描画
	objectManager->Draw();
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


