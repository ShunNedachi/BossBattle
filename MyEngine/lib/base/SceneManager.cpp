#include"SceneManager.h"
#include"TitleScene.h"

// Ã“Iƒƒ“ƒo•Ï”
SceneManager* SceneManager::instance = nullptr;
Scene* SceneManager::nowScene = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new SceneManager();
		nowScene = new TitleScene();
	}

	return instance;
}

void SceneManager::Destroy()
{
	delete instance;
	delete nowScene;

	instance = nullptr;
	nowScene = nullptr;
}

void SceneManager::NextScene()
{
	nowScene->NextScene(this);
}

void SceneManager::ChangeScene(Scene* scene)
{
	delete nowScene;
	nowScene = scene;
	nowScene->Initalize();
}
