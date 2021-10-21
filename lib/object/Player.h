#pragma once
#include"Object.h"
#include"Sprite2D.h"
#include"AttackBase.h"
#include<string>

class Player
{
	// シングルトンパターン

private:
	// エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using String = std::string;

public:
	// 関数

	static Player* GetInstance();
	void Destroy();

	//
	void Init(const String& filename);
	void Update();
	void Draw();

	// setter
	void SetPos(XMFLOAT3 pos) { position = pos; }
	void SetRot(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scale) { scale = scale; }

	// getter
	XMFLOAT3 GetPos() { return position; }
	XMFLOAT3 GetRot() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }


protected:

private:
	// コンストラクタ
	Player() = default;
	// デストラクタ
	~Player() = default;

	// 禁止項目
	void operator=(const Player& obj) {}
	Player(const Player& obj){}

private:
	// 変数

	static Player* instance;

	Object* objPlayer = nullptr;
	XMFLOAT3 position = {0,0,0};
	XMFLOAT3 rotation = {0,0,0};
	XMFLOAT3 scale = {1,1,1};

	// system用
	float health;

	// 攻撃用
	AttackBase* attack;
	Object* attackObj;

	// 回転確認用
	float rotX = 0; // x軸用
	float rotY = 0; // y軸用

};

