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

	objectManager->AddSprite(clearSprite, { SETTING_VALUE::WINDOW_WIDTH / 2,SETTING_VALUE::WINDOW_HEIGHT / 2 });
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
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();

	for (int i = 0; i < spriteArray.size(); i++)
	{
		spriteArray[i]->Draw();
	}
}

void ClearScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new TitleScene());
}
