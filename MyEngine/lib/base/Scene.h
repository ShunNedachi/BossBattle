#pragma once

#define titleSceneNum 0
#define gameSceneNum 1
#define endSceneNum 2

class SceneManager;


// シーンクラスの基底クラス
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// シーンの動作用
	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// シーンの切り替え
	virtual void NextScene(SceneManager* nowScene) = 0;
};

