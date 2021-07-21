#include "GameScene.h"

void GameScene::Initalize()
{
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


