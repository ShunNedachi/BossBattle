#include "EndScene.h"


EndScene::~EndScene()
{
	objectManager->Destroy();
}

void EndScene::Initalize()
{
	objectManager->AddSprite(0, "gameOver.png", { WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 });
}


void EndScene::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	if (IsNext())NextScene(SceneManager::GetInstance());

	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButton(0)& XINPUT_BUTTON_A)isNext = true;

}

void EndScene::Draw()
{
	objectManager->Draw();
}

void EndScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}

