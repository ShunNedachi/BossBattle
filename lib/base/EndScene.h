#pragma once
#include "Scene.h"
#include"TitleScene.h"
#include"GameScene.h"


class EndScene :
    public Scene
{
	~EndScene();

	// �V�[���̓���p
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// �V�[���̐؂�ւ�
	void NextScene(SceneManager* nowScene) override;

private:
};

