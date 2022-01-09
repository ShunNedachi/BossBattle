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
    
	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;

private:
	// メンバ変数
};

