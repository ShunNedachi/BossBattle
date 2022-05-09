#pragma once
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include"Sprite2D.h"
#include"Camera.h"
#include "XinputControll.h"
#include"SceneManager.h"
#include"FrameFixed.h"
#include"Light.h"
#include"PostEffect.h"

#include"ParticleManager.h"
#include"Particle2D.h"


class Application
{
public:

	static void Initialize();
	static void Destroy();

	static void Update();

	static void Draw();

private:
	// �E�B���h�E�p
	static MyWindow myw;
	// directX12
	static MyDirectX12 my12;

	// DirectInput,Xinput�p
	static Input* input;
	static Xinput* xinput;

	// �V�[���Ǘ��p
	static SceneManager* sceneManager;
	// fps�Œ�p
	static FrameFixed* frameFixed;

};

