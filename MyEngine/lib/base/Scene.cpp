#include"Scene.h"

// �ÓI�����o�ϐ�
Scene* Scene::instance = nullptr;
Scenes Scene::nowScene = title;

Scene* Scene::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Scene;
	}

	return instance;
}

void Scene::Destroy()
{
	delete instance;

	instance = nullptr;
}

void Scene::NextScene()
{
	if (nowScene == title)nowScene = game;

}
