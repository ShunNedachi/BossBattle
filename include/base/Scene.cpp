#include"Scene.h"
#include"SceneManager.h"


void Scene::ChangeScene(SceneManager* nowScene, int sceneNum)
{
	if (sceneNum == titleSceneNum)nowScene->ChangeScene(new TitleScene());
	else if (sceneNum == gameSceneNum)nowScene->ChangeScene(new GameScene());
	else if (sceneNum == endSceneNum)nowScene->ChangeScene(new EndScene());
	else if (sceneNum == clearSceneNum)nowScene->ChangeScene(new ClearScene());
}
