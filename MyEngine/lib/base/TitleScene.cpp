#include "TitleScene.h"

void TitleScene::Initalize()
{
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
}

void TitleScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new GameScene());
}


