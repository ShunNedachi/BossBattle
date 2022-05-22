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

	// �X�v���C�g�ǉ�
	objectManager->AddSprite(titleSprite,{ SETTING_VALUE::WINDOW_WIDTH / 2,SETTING_VALUE::WINDOW_HEIGHT / 2 - 200 });
	objectManager->AddSprite(titleExplainSprite,{650,600});
	objectManager->AddSprite(selectTutorial, { 650,500 });
	objectManager->AddSprite(selectGameStart, { 650,650 });
	objectManager->AddSprite(arrowSprite, { 400,500 });
	keepArrowPos = { 400,500 };

	// �T�C�Y�ύX
	Sprite2D* spriteTemp = objectManager->GetSpriteArray()->at((int)TitleTexNum::titleExplain);
	spriteTemp->Resize(400, 200);
}

void TitleScene::Update()
{
	// input�֌W
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();


	// �X�^�[�g����������̏���
	if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))
	{
		startAnimation = true;
		spriteArray[(int)TitleTexNum::titleExplain]->SetFlashSpeed(5);
	}


	// �X�^�[�g�Ń{�^���������ꂽ�Ƃ����I����Ԃ���Ȃ��Ƃ��@���点��
	if (startAnimation && !isSelect)spriteArray[(int)TitleTexNum::titleExplain]->SetDrawFlash(true);

	if (startAnimation && spriteArray[(int)TitleTexNum::titleExplain]->GetAlpha() < 0) 
	{
		// �����Ă���̂��~�߂�
		spriteArray[(int)TitleTexNum::titleExplain]->SetDrawFlash(false);

		isSelect = true;
		startAnimation = false;

		// ���̃A�j���[�V�����̏�����
		arrowEasing.SetState(1, 400, 425);
	}


	// �I�𒆂̏���
	if (isSelect)
	{
		// ���̏㉺�ړ�
		if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_UP || input->TriggerKey(DIK_UP))
		{
			// ��ԏ�ɂȂ����ɏ�Ɉړ��\
			if (selectMenu > (int)SelectMenu::tutorial)
			{
				arrowFlg = true;
				selectMenu--;
			}
		}
		else if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_DOWN || input->TriggerKey(DIK_DOWN))
		{
			// ��ԉ��ɂȂ����ɉ��Ɉړ��\
			if (selectMenu < (int)SelectMenu::gamestart)
			{
				arrowFlg = true;
				selectMenu++;
			}
		}

		// �ړ�����ꍇ
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

			// ���̃A�j���[�V�����̏�����
			arrowEasing.SetState(1, 400, 425);


			arrowFlg = false;
		}


		// ���̃A�j���[�V����
		float setPosY = spriteArray[(int)TitleTexNum::arrow]->GetPosition().y;
		spriteArray[(int)TitleTexNum::arrow]->SetPosition({ arrowEasing.StartEeaging(easeInFLAG),setPosY });
		// �C�[�W���O���I�����Ă����猳�̈ʒu�ɖ߂�
		if (arrowEasing.GetEndFlg())
		{
			spriteArray[(int)TitleTexNum::arrow]->SetPosition(keepArrowPos);
			// ���̃A�j���[�V�����̏�����
			arrowEasing.SetState(1, 400, 425);
		}



		// �I�����ꂽ�Ƃ��Ɏ��̃V�[���Ɉړ�
		if (xinput->TriggerButton(CONTROLLER_1) & XINPUT_BUTTON_A || input->TriggerKey(DIK_SPACE))
		{
			isNext = true;
		}
	}


	// �X�v���C�g�̍X�V����
	for (auto x : spriteArray)
	{
		x->Update();
	}

	if(IsNext())NextScene(SceneManager::GetInstance());
}

void TitleScene::Draw()
{
	std::vector<Sprite2D*> spriteArray = *objectManager->GetSpriteArray();

	// �^�C�g�������̕`��
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


