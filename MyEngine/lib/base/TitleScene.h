#pragma once
#include "Scene.h"
#include"GameScene.h"
#include"EndScene.h"
#include"SceneManager.h"



class TitleScene :
    public Scene
{
    ~TitleScene(){};
    
	// �V�[���̓���p
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// �V�[���̐؂�ւ�
	void NextScene(SceneManager* nowScene) override;
};

