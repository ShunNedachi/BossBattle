#include<DirectXMath.h>
#pragma once

// �N���X�Ԃł̏�������l�̎󂯓n����objectManager��ʂ����s��
// �܂��������܂Ƃ߂邽�߂Ɏg�p
namespace GameFunction
{
	void AddEXP(float exp);

	void LoadGameSceneTexture();

	DirectX::XMFLOAT3 GetPlayerPos();
	bool GetPlayerIsSpecial();

}
