#include "TitleScene.h"
#include"Input.h"
#include"XinputControll.h"
#include"ObjectManager.h"

TitleScene::~TitleScene()
{
	objectManager->Destroy();
}

void TitleScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();

	objectManager->AddSprite(0, "title.png", {650,600});
}

void TitleScene::Update()
{
	// inputŠÖŒW
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	
	if(IsNext())NextScene(SceneManager::GetInstance());
		
	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButton(0)& XINPUT_BUTTON_A)isNext = true;
		

	objectManager->Update();
}

void TitleScene::Draw()
{
	objectManager->Draw();
}

void TitleScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new GameScene());
}


