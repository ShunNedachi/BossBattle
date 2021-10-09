#pragma once
#include"Object.h"
#include"Sprite2D.h"
#include<string>

class Player
{
	// シングルトンパターン

private:
	// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using String = std::string;

public:
	// 関数

	static Player* GetInstance();
	static void Destroy();

	//
	static void Init(const std::string& filename);
	void Update();
	void Draw();

	// setter
	void SetPos(XMFLOAT3 pos) { playerPos = pos; }
	void SetRot(XMFLOAT3 rot) { playerRot = rot; }
	void SetScale(XMFLOAT3 scale) { playerScale = scale; }

	// getter
	XMFLOAT3 GetPos() { return playerPos; }
	XMFLOAT3 GetRot() { return playerRot; }
	XMFLOAT3 GetScale() { return playerScale; }


protected:
	// コンストラクタ
	Player() {};
	// デストラクタ
	~Player() {};


private:
	// 変数

	static Player* instance;

	static Object* objPlayer;
	static XMFLOAT3 playerPos;
	static XMFLOAT3 playerRot;
	static XMFLOAT3 playerScale;

};

