#pragma once
#include"ObjectManager.h"
#include"GameFunction.h"


#define titleSceneNum 0
#define gameSceneNum 1
#define endSceneNum 2
#define clearSceneNum 3

class SceneManager;


// �V�[���N���X�̊��N���X
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	// �V�[���̓���p
	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// �V�[���̐؂�ւ�
	virtual void NextScene(SceneManager* nowScene) = 0;
	void ChangeScene(SceneManager* nowScene,int sceneNum);

	// �؂�ւ��v���p
	bool IsNext() { return isNext; }

protected:

	bool isNext = false;
	ObjectManager* objectManager;
};

