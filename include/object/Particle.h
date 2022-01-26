#pragma once
#include<vector>
#include<DirectXTex.h>
#include"MyDirectX12.h"
#include"Camera.h"
#include<string>
#include"Model.h"


#define NORMAL 0
#define TOON 1
class Particle
{
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:


	Particle();
	~Particle();

#pragma region メンバー関数

	// 初期化処理　全体で一回のみinitを回す
	static void Init(MyDirectX12* my12);
	static void CreatePiplineStateOBJ();


	void Draw();
	void DrawFlash();

	void Update();

	void CreateModel();
	void LoadMaterial(const string& directryPath, const string& filename);
	bool LoadTexture(const string& directoryPath, const string& filename);


	// setter
	//void SetModel(Model* model) { this->model = model; }
	void SetShaderNum(int shaderNum) { this->shaderNum = shaderNum; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetSize(XMFLOAT3 size) { this->size = size; }
	void SetRadius(float r) { this->r = r; }
	void SetBillboard(bool flg) { isBillboard = flg; }
	void SetBillboardY(bool flg) { isBillboardY = flg; }


	// getter
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetSize() { return size; }
	float GetRadius() { return r; }


#pragma endregion

private:

#pragma region 構造体

	struct ConstBufferDataB0
	{
		XMMATRIX mat; // 3d変換行列
		XMFLOAT3 cameraPos;
		XMFLOAT3 color;
		//float colorR; // 色（RGBA）
		//float colorG;
		//float colorB;
		//float colorA;
	};

	// 定数バッファ用データ構造体b1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};
	struct Vertex
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv; //	uv座標
	};

	struct Material
	{
		string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float alpha;
		string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
#pragma endregion


#pragma region	directX

	// 共有する変数
	static ComPtr<ID3D12RootSignature> rootSignature[2]; // ルートシグネチャ
	static ComPtr<ID3D12PipelineState> pipelineState[2]; // パイプラインステート
	static XMMATRIX matProjection; // 射影行列
	static const int SRVCount = 512; // テクスチャの最大枚数
	static XMMATRIX matView;

	static ComPtr<ID3D12GraphicsCommandList> commandList;
	static ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;



	// 1枚ごとに変える変数
	ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	ComPtr<ID3D12Resource> indexBuff; // インデックスバッファ
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;

	ComPtr<ID3D12Resource> texBuff;

	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW  ibView{}; // インデックスバッファビュー

#pragma endregion


// eye targetはコンストラクタ内部
	static Camera* camera;
	static const int vertexCount = 4;
	static const int indexCount = 3 * 2;

	// 頂点データ配列
	static Vertex vertices[vertexCount];
	static unsigned short indices[indexCount];


	XMMATRIX matWorld; // ワールド行列
	XMMATRIX matScale; // スケーリング行列
	XMMATRIX matRot; // 回転行列
	XMMATRIX matTrans; // 平行移動行列

	XMFLOAT3 scale = { 1,1,1 };     // スケール


	UINT texNumber = 0; //	テクスチャ番号

	// どちらかに値を入れてオブジェクトの大きさを取ってくる(主に当たり判定用)
	XMFLOAT3 size = { 0,0,0 }; // 矩形の大きさ
	float r = 0; // 半径



	int shaderNum;
	Material material;

	// ビルボード用
	bool isBillboard = false;
	bool isBillboardY = false;


};
