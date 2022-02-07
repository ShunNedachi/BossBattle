#pragma once
#include "Scene.h"
#include"GameScene.h"
#include"EndScene.h"
#include"SceneManager.h"
#include"ObjectManager.h"

class TitleScene :
    public Scene
{
public:
    ~TitleScene();
    
	// シーンの動作用
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// シーンの切り替え
	void NextScene(SceneManager* nowScene) override;
	
private:
	// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;

	//　スタート後の選択で何を選択している物
	enum SelectMenu
	{
		tutorial,
		gamestart,
	};

private:
	// メンバ変数
	
	// スタート後の選択用
	bool isSelect = false;
	// 矢印の位置調整用
	bool arrowFlg = false;
	// 何が選択されているのか 万が一択が増えてもいいようにint
	int selectMenu = tutorial;
	
	// 矢印のアニメーション用
	Easing arrowEasing;
	XMFLOAT2 keepArrowPos;

	// アニメーション用
	bool startAnimation = false;
};

