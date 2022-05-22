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

	// スプライト追加
	objectManager->AddSprite(titleSprite,{ SETTING_VALUE::WINDOW_WIDTH / 2,SETTING_VALUE::WINDOW_HEIGHT / 2 - 200 });
	objectManager->AddSprite(titleExplainSprite,{650,600});
	objectManager->AddSprite(selectTutorial, { 650,500 });
	objectManager->AddSprite(selectGameStart, { 650,650 });
	objectManager->AddSprite(arrowSprite, { 400,500 });
	keepArrowPos = { 400,500 };

	// サイズ変更
	Sprite2D* spriteTemp = objectManager->GetSpriteArray()->at((int)TitleTexNum::titleExplain);
	spriteTemp->Resize(400, 200);
}

void TitleScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();


	// スタートを押した後の処理
	if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))
	{
		startAnimation = true;
		spriteArray[(int)TitleTexNum::titleExplain]->SetFlashSpeed(5);
	}


	// スタートでボタンを押されたときかつ選択状態じゃないとき　光らせる
	if (startAnimation && !isSelect)spriteArray[(int)TitleTexNum::titleExplain]->SetDrawFlash(true);

	if (startAnimation && spriteArray[(int)TitleTexNum::titleExplain]->GetAlpha() < 0) 
	{
		// 光っているのを止める
		spriteArray[(int)TitleTexNum::titleExplain]->SetDrawFlash(false);

		isSelect = true;
		startAnimation = false;

		// 矢印のアニメーションの初期化
		arrowEasing.SetState(1, 400, 425);
	}


	// 選択中の処理
	if (isSelect)
	{
		// 矢印の上下移動
		if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_UP || input->TriggerKey(DIK_UP))
		{
			// 一番上にない時に上に移動可能
			if (selectMenu > (int)SelectMenu::tutorial)
			{
				arrowFlg = true;
				selectMenu--;
			}
		}
		else if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_DOWN || input->TriggerKey(DIK_DOWN))
		{
			// 一番下にない時に下に移動可能
			if (selectMenu < (int)SelectMenu::gamestart)
			{
				arrowFlg = true;
				selectMenu++;
			}
		}

		// 移動する場合
		if (arrowFlg)
		{
			if (selectMenu == (int)SelectMenu::tutorial)
			{
				keepArrowPos.y = spriteArray[(int)TitleTexNum::tutorial]->GetPosition().y;
			}
			else if (selectMenu == (int)SelectMenu::gamestart)
			{
				keepArrowPos.y = spriteArray[(int)TitleTexNum::gamestart]->GetPosition().y;
			}
			spriteArray[(int)TitleTexNum::arrow]->SetPosition(keepArrowPos);

			// 矢印のアニメーションの初期化
			arrowEasing.SetState(1, 400, 425);


			arrowFlg = false;
		}


		// 矢印のアニメーション
		float setPosY = spriteArray[(int)TitleTexNum::arrow]->GetPosition().y;
		spriteArray[(int)TitleTexNum::arrow]->SetPosition({ arrowEasing.StartEeaging(easeInFLAG),setPosY });
		// イージングが終了していたら元の位置に戻す
		if (arrowEasing.GetEndFlg())
		{
			spriteArray[(int)TitleTexNum::arrow]->SetPosition(keepArrowPos);
			// 矢印のアニメーションの初期化
			arrowEasing.SetState(1, 400, 425);
		}



		// 選択されたときに次のシーンに移動
		if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))
		{
			isNext = true;
		}
	}


	// スプライトの更新処理
	for (auto x : spriteArray)
	{
		x->Update();
	}

	if(IsNext())NextScene(SceneManager::GetInstance());
}

void TitleScene::Draw()
{
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();

	// タイトル文字の描画
	spriteArray[(int)TitleTexNum::title]->Draw();

	if (!isSelect)
	{
		spriteArray[(int)TitleTexNum::titleExplain]->Draw();
	}
	else
	{
		spriteArray[(int)TitleTexNum::tutorial]->Draw();
		spriteArray[(int)TitleTexNum::gamestart]->Draw();
		spriteArray[(int)TitleTexNum::arrow]->Draw();
	}
	
}

void TitleScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new GameScene());
}


