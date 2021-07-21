#pragma once
#include"Scene.h"
#include"TitleScene.h"


// シングルトンパターン
class SceneManager
{
public:
	// 
	static SceneManager* GetInstance();
	static void Destroy();

	// シーンの設定
	void NextScene();
	void ChangeScene(Scene* scene);

protected:
	SceneManager() {};
	~SceneManager() { };

private:

	static SceneManager* instance;
	static Scene* nowScene; // 現在のシーン
};



