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

namespace ApplicationSetting
{
	const std::string WINDOW_NAME = "BOSS BATTLE";
}

// シングルトンパターン
class Application
{
public:

	static Application* GetInstance();

	void Initialize();
	void Destroy();

	void Update();

	void Draw();


private:

	// コンストラクタ　デストラクタ
	Application() = default;
	~Application() = default;

	void operator=(const Application& obj) = delete;
	Application(const Application& obj) = delete;

	// ウィンドウクラスの初期化
	void WindowInitialize();
	// directXクラスの初期化
	void DXInitialize();
	// インプットクラスの初期化
	void InputInitialize();
	// マネージャークラスの初期化
	void ManagerInitialize();
	// パーティクルクラスの初期化
	void ParticleInitialize();

public:

	static Application* instance;

	// ウィンドウ用
	std::shared_ptr<MyWindow> window;
	// directX12
	std::shared_ptr<MyDirectX12> directX;
	

	std::shared_ptr<std::string> windowName;
};

