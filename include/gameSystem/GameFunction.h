#pragma once
#include<DirectXMath.h>
#include"Enemy.h"
#include"SpriteLoadDefine.h"


// クラス間での情報交換や値の受け渡しをobjectManagerを通さず行う
// また処理をまとめるために使用
namespace GameFunction
{
	// タイトルシーン画像番号保存用
	enum class TitleTexNum
	{
		title,
		titleExplain,
		tutorial,
		gamestart,
		arrow,
	};

	void AddEXP(float exp);

	void LoadGameSceneTexture();
	void LoadTitleSceneTexture();
	void LoadEndSceneTexture();
	void LoadClearSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();


	// 念のため処理区分用
	void LoadPlayerTexture();
	void LoadBossTexture();
	void LoadNumberTexture();
}
