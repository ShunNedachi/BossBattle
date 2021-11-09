#pragma once
#include "Scene.h"
#include"TitleScene.h"
#include"EndScene.h"
#include<ObjectManager.h>
#include"Camera.h"
#include"Player.h"
#include"Model.h"

class GameScene :
    public Scene
{
	~GameScene();

	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;

public:

	// 関数

private:

	// カメラ
	Camera* camera;

};

