#pragma once
#include "Scene.h"
#include"TitleScene.h"

class ClearScene :
    public Scene
{
public:

	~ClearScene();

	// �V�[���̓���p
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// �V�[���̐؂�ւ�
	void NextScene(SceneManager* nowScene) override;

};

