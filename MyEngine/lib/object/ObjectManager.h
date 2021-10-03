#pragma once
#include<vector>
#include"Object.h"
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Sprite2D.h"

// シングルトンパターン
class ObjectManager
{
public:
	// エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	ObjectManager() {};
	~ObjectManager() {};

	// コンストラクタ　デストラクタ
	static ObjectManager* GetInstance();
	static void Destroy();
	// 初期化
	static void Initialize(MyDirectX12* my12, MyWindow* window, Camera* camera);

	void Update();
	void Draw();

	#pragma region 3Dオブジェクト用関数

	// オブジェクト追加コマンド
	void AddOBJ(const std::string& filename, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = {1,1,1}, 
		XMFLOAT3 rotation = { 0,0,0 },int drawShader = NORMAL);

	#pragma endregion


	#pragma region スプライト用関数

	// 画像データロード
	void LoadTexture(const wchar_t* filename, int textureNum);

	// スプライト生成
	void AddSprite(int textureNum, XMFLOAT2 position = { 0,0 }, float rotation = 0.0f, 
		XMFLOAT2 anchorPoint = {0.5f,0.5f}, XMFLOAT4 color = { 1,1,1,1 });


#pragma endregion



private:

	static ObjectManager* instance;

	// .obj用配列
	static std::vector<Object*> objArray;
	// sprite用配列
	static std::vector<Sprite2D*> spriteArray;


};

