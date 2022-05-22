#pragma once
#include "Scene.h"
#include"TitleScene.h"
#include"GameScene.h"


class EndScene :
    public Scene
{
	~EndScene();

	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;

private:
};

