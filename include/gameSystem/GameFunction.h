#pragma once
#include<DirectXMath.h>
#include"Enemy.h"
#include"SpriteLoadDefine.h"


// �N���X�Ԃł̏�������l�̎󂯓n����objectManager��ʂ����s��
// �܂��������܂Ƃ߂邽�߂Ɏg�p
namespace GameFunction
{
	void AddEXP(float exp);

	void LoadGameSceneTexture();
	void LoadTitleSceneTexture();
	void LoadEndSceneTexture();
	void LoadClearSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();


	// �O�̂��ߏ����敪�p
	void LoadPlayerTexture();
	void LoadBossTexture();
	void LoadNumberTexture();
}
