#pragma once
#include"Scene.h"
#include"TitleScene.h"


// �V���O���g���p�^�[��
class SceneManager
{
public:
	// 
	static SceneManager* GetInstance();
	static void Destroy();

	// �V�[���̐ݒ�
	void NextScene();
	void ChangeScene(Scene* scene);

protected:
	SceneManager() {};
	~SceneManager() { };

private:

	static SceneManager* instance;
	static Scene* nowScene; // ���݂̃V�[��
};



