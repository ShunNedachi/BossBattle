#include "EndScene.h"

void EndScene::Initalize()
{
}

void EndScene::Update()
{
}

void EndScene::Draw()
{
}

void EndScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}

