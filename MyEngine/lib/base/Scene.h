#pragma once

#define titleSceneNum 0
#define gameSceneNum 1
#define endSceneNum 2

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
};

