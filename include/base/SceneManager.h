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

	// シーン切り替え要求受け取り用
	bool canChange() { return nowScene->IsNext(); }

	// 関数
	void Update();
	void Draw();


protected:
	SceneManager() {};
	~SceneManager() { };

private:

	static SceneManager* instance;
	static Scene* nowScene; // 現在のシーン

	bool isNext = false;
};



