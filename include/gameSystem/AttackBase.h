#pragma once
#include<DirectXMath.h>
#include"Object.h"
#include"Light.h"

struct AttackSize
{
	DirectX::XMFLOAT3 squareSize = {0,0,0};
	float ballRadius = 0;
};

class AttackBase
{
private:
	// エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;


public:
	// 関数


	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="startFrame">攻撃前の待機フレーム</param>
	/// <param name="stopFrame">攻撃後の硬直フレーム</param>
	/// <param name="attackFrame">攻撃用の持続フレーム</param>
	/// <param name="size">攻撃の範囲</param>
	/// <param name="comboInputTime"></param>
	AttackBase(int startFrame,int attackFrame, int stopFrame,int comboInputTime = 0);
	~AttackBase();

	// update
	void Update();
	void Draw(Light& light);

	// 攻撃開始用
	void SetAttackStart();
	void SetObject(Object* object) { this->object = object; }

	// get
	XMFLOAT3 GetAttackPos() { return object->GetPosition(); }
	bool GetIsAttack() { return isAttack; }
	bool GetAttackStart() { return attackStart; }

	/// <summary>
	/// 攻撃が当たっているのか
	/// </summary>
	/// <param name="pos">判定したいものの位置</param>
	/// <param name="size">矩形のサイズ</param>
	/// <param name="r">半径</param>
	/// <returns></returns>
	bool Hit(XMFLOAT3 pos, XMFLOAT3 size = { 0,0,0 }, float r = 0);

private:

	// 攻撃開始
	bool attackStart = false;

	// 攻撃前の待機フレーム
	int startFrame = 0;
	bool isStart = false;
	// 攻撃の持続フレーム
	int attackFrame = 0;

	// 攻撃中かどうか
	bool isAttack = false;


	// 硬直中かどうか
	bool isEnd = false;
	// 攻撃後の硬直フレーム
	int stopFrame = 0;
	
	// 連続攻撃用受付時間
	float comboInputTime = 0;
	
	//// 攻撃の判定範囲用
	//AttackSize size;

	// 経過フレーム(計算用)
	int elapsedFrame = 0;
	
	// 攻撃の見た目用
	Object* object = nullptr;

	//
};

