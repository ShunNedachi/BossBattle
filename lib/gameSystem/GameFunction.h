#include<DirectXMath.h>
#include"Enemy.h"
#pragma once

// �N���X�Ԃł̏�������l�̎󂯓n����objectManager��ʂ����s��
// �܂��������܂Ƃ߂邽�߂Ɏg�p
namespace GameFunction
{
	void AddEXP(float exp);

	void LoadGameSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();


	// �O�̂��ߏ����敪�p
	void LoadPlayerTexture();
	void LoadBossTexture();
	void LoadNumberTexture();
}
