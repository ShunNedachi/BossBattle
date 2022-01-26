#pragma once
#include "Scene.h"
#include"TitleScene.h"

class ClearScene :
    public Scene
{
public:

	~ClearScene();

	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;

};

