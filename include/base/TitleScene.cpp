#include "TitleScene.h"
#include"Input.h"
#include"XinputControll.h"
#include"ObjectManager.h"
#include"GameFunction.h"

TitleScene::~TitleScene()
{
	objectManager->Destroy();
}

void TitleScene::Initalize()
{
	objectManager = ObjectManager::GetInstance();

	GameFunction::LoadTitleSceneTexture();

	objectManager->AddSprite(titleExplainSprite,{650,600});
	objectManager->SetSize(0, { 400,200 });
	objectManager->AddSprite(titleSprite,{WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 - 200 });

}

void TitleScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	std::vector<Object*> objArray = *objectManager->GetObjectArray();
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();

	// スタートを押した後の処理
	if (xinput->TriggerButton(0) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))
	{
		startAnimation = true;
		objectManager->SetFlashSpeed(0, 5);
	}


	// スタートでボタンを押されたときかつ選択状態じゃないとき　光らせる
	if (startAnimation && !isSelect)objectManager->SpriteFlash(0, true);

	if (startAnimation && objectManager->GetColor(0).w < 0) 
	{
		// 光っているのを止める
		objectManager->SpriteFlash(0, false);

		isSelect = true;
		startAnimation = false;
	}

	if(IsNext())NextScene(SceneManager::GetInstance());

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


