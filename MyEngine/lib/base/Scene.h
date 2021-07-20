#pragma once
#include"Audio.h"

enum Scenes
{
	title,
	game,
	gameover,
	clear
};

// �V���O���g���p�^�[��
class Scene
{
public:
	// 
	static Scene* GetInstance();
	static void Destroy();

	// �V�[���̐ݒ�
	void NextScene();

protected:
	Scene() {};
	~Scene() {};

private:

	static Scene* instance;
	static Scenes nowScene;
};

