#pragma once
#include "Scene.h"
#include"GameScene.h"
#include"EndScene.h"
#include"SceneManager.h"



class TitleScene :
    public Scene
{
    ~TitleScene(){};
    
	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;
};

