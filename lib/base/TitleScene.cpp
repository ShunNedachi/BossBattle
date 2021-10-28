#include "TitleScene.h"
#include"Input.h"
#include"XinputControll.h"
#include"ObjectManager.h"

TitleScene::~TitleScene()
{
	objManager->Destroy();
}

void TitleScene::Initalize()
{
	objManager = ObjectManager::GetInstance();

	objManager->AddSprite(0, "title.png", {500,600});
}

void TitleScene::Update()
{
	// inputŠÖŒW
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	
	if(IsNext())NextScene(SceneManager::GetInstance());
		
	// ƒV[ƒ“•ÏX
	if (xinput->TriggerButtom(0, xinput_A))isNext = true;
		

	objManager->Update();
}

void TitleScene::Draw()
{
	objManager->Draw();
}

void TitleScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new GameScene());
}


