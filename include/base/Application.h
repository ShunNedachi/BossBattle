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
	// ウィンドウ用
	static MyWindow myw;
	// directX12
	static MyDirectX12 my12;

	// DirectInput,Xinput用
	static Input* input;
	static Xinput* xinput;

	// シーン管理用
	static SceneManager* sceneManager;
	// fps固定用
	static FrameFixed* frameFixed;

};

