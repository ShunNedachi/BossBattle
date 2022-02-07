#include "ClearScene.h"
#include"GameFunction.h"

ClearScene::~ClearScene()
{
	objectManager->Destroy();
}

void ClearScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();


	GameFunction::LoadClearSceneTexture();

	objectManager->AddSprite(clearSprite, { WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 });

}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	if (IsNext())NextScene(SceneManager::GetInstance());

	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))isNext = true;
}

void ClearScene::Draw()
{
	objectManager->Draw();
}

void ClearScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}
