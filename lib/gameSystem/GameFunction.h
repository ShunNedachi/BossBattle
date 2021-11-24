#include<DirectXMath.h>
#pragma once

// クラス間での情報交換や値の受け渡しをobjectManagerを通さず行う
// また処理をまとめるために使用
namespace GameFunction
{
	void AddEXP(float exp);

	void LoadGameSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();

}
