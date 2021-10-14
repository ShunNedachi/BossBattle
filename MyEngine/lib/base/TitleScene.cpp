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
	// input�֌W
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// �V�[���ύX
	NextScene(SceneManager::GetInstance());

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


