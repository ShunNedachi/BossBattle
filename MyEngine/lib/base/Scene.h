#pragma once
#include"Audio.h"

enum Scenes
{
	title,
	game,
	gameover,
	clear
};

// シングルトンパターン
class Scene
{
public:
	// 
	static Scene* GetInstance();
	static void Destroy();

	// シーンの設定
	void NextScene();

protected:
	Scene() {};
	~Scene() {};

private:

	static Scene* instance;
	static Scenes nowScene;
};

