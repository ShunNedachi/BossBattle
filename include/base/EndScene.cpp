#include "EndScene.h"
#include"GameFunction.h"


EndScene::~EndScene()
{
	objectManager->Destroy();
}

void EndScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();

	GameFunction::LoadEndSceneTexture();
	objectManager->AddSprite(endSprite, { WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 });
}


void EndScene::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	if (IsNext())NextScene(SceneManager::GetInstance());

	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButton(CONTROLLER_1)& XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))isNext = true;
}

void EndScene::Draw()
{
	objectManager->Draw();
}

void EndScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}

