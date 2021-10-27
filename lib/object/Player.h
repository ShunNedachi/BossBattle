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

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="pos">攻撃者の位置</param>
	/// <param name="damage"></param>
	void RecieveDamage(XMFLOAT3 pos, float damage);

	// setter 後で修正
	void SetPos(XMFLOAT3 pos) { position = pos; }
	void SetRot(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scale) { scale = scale; }
	void SetSize(XMFLOAT3 size) { objPlayer->SetSize(size); }
	void SetRadius(float r) { objPlayer->SetRadius(r); }

	// getter
	XMFLOAT3 GetPos() { return position; }
	XMFLOAT3 GetRot() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetSize() { return objPlayer->GetSize(); }
	XMFLOAT3 GetAttackPos() { return attack->GetAttackPos(); }
	float GetRadius() { return objPlayer->GetRadius(); }
	bool GetIsDamage() { return isDamage; }
	int GetDamage() { return damage; }

	// 攻撃判定用
	AttackBase* GetAttack() { return attack; }
	Object* GetOBJ() { return objPlayer; }


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
	// helth値
	float health = 10;
	// 経験値量
	float experience;
	// ダメージをくらっているのかの判定
	bool isDamage = false;
	// ダメージ状態の経過フレーム数
	int damageCount;
	// ダメージ状態の最大フレーム数
	const int DAMAGE_FRAME = 20;

	int damage = 1;

	// 体力用
	Sprite2D* healthBarSprite = nullptr;
	// 体力バー用
	std::vector<Sprite2D*> healthSprite;


	// 攻撃用
	AttackBase* attack;
	Object* attackObj;

	// プレイヤーの正面ベクトル
	XMVECTOR move;

};

