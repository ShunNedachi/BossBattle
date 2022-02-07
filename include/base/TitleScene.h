#pragma once
#include "Scene.h"
#include"GameScene.h"
#include"EndScene.h"
#include"SceneManager.h"
#include"ObjectManager.h"

class TitleScene :
    public Scene
{
public:
    ~TitleScene();
    
	// �V�[���̓���p
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// �V�[���̐؂�ւ�
	void NextScene(SceneManager* nowScene) override;
	
private:
	// �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;

	//�@�X�^�[�g��̑I���ŉ���I�����Ă��镨
	enum SelectMenu
	{
		tutorial,
		gamestart,
	};

private:
	// �����o�ϐ�
	
	// �X�^�[�g��̑I��p
	bool isSelect = false;
	// ���̈ʒu�����p
	bool arrowFlg = false;
	// �����I������Ă���̂� ��������������Ă������悤��int
	int selectMenu = tutorial;
	
	// ���̃A�j���[�V�����p
	Easing arrowEasing;
	XMFLOAT2 keepArrowPos;

	// �A�j���[�V�����p
	bool startAnimation = false;
};

