#pragma once
#include<vector>
#include"Object.h"
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Sprite2D.h"
#include"Player.h"
#include<string>
#include"Model.h"
#include"Boss.h"
#include"Light.h"
#include"ParticleManager.h"

// シングルトンパターン
class ObjectManager
{
private:
	// エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;


private:
	ObjectManager() = default;
	~ObjectManager() = default;

	void operator=(const ObjectManager& obj) = delete;
	ObjectManager(const ObjectManager& obj) = delete;

	// private関数
	void CheckArray();

public:


	static ObjectManager* GetInstance();
	static void Destroy();

	static void Initialize(MyDirectX12* my12);

	// オブジェクトの受け渡し用
	// 後でこれらをscene内に持って行ってupdateを各自でするように変更する
	static std::vector<Object*>* GetObjectArray() { return &objArray; }
	static std::vector<Sprite2D*>* GetSpriteArray() { return &spriteArray; }
	static Player* GetPlayer() { return player; }
	static Boss* GetBoss() { return boss; }
	static Light* GetLight() { return light; }

	void Update();
	//void Draw();

	#pragma region Player関係関数

	static void AddPlayer(const std::string& filename);

#pragma endregion


	#pragma region Enemy関係関数

	void AddBoss();

#pragma endregion


	#pragma region 3Dオブジェクト用関数

	// オブジェクト追加コマンド
	void AddOBJ(const std::string& filename, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = {1,1,1}, 
		XMFLOAT3 rotation = { 0,0,0 },int drawShader = NORMAL);

	void DeleteOBJ(int 
	
	);

	#pragma endregion


	#pragma region スプライト用関数

	// 画像データロード
	//void LoadTexture(const wchar_t* filename, int textureNum);

	// スプライト生成
	//void AddSprite(int textureNum, const std::string& filename = "white.png", XMFLOAT2 position = { 0,0 }, float rotation = 0.0f,
	//	XMFLOAT2 anchorPoint = { 0.5f,0.5f }, XMFLOAT4 color = { 1,1,1,1 });

	void AddSprite(int textureNum, XMFLOAT2 position = { 0,0 }, float rotation = 0.0f,
		XMFLOAT2 anchorPoint = { 0.5f,0.5f }, XMFLOAT3 color = { 1,1,1 });

	// 削除
	void DeleteSprite(int index);

#pragma endregion



private:

	static Xinput* xinput;

	static ObjectManager* instance;

	// .obj用配列
	static std::vector<Object*> objArray;


	// sprite用配列
	static std::vector<Sprite2D*> spriteArray;

	// ロードされているファイルデータを保存
	//static std::vector<std::string> loadedFileArray;

	static Player* player;

	// Enemy 仮置き
	static Boss* boss;

	// ライト　複数光源は後程対応するかも
	static Light* light;

	// クリア条件達成確認用
	bool isClear = false;
};

