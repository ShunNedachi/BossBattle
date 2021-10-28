#include "EndScene.h"

void EndScene::Initalize()
{
}

void EndScene::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	if (IsNext())NextScene(SceneManager::GetInstance());

	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButtom(0, xinput_A))isNext = true;

}

void EndScene::Draw()
{
}

void EndScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}

