#pragma once
#include<vector>
#include<DirectXTex.h>
#include"MyDirectX12.h"
#include"Camera.h"
#include<string>
#include"Model.h"
#include"Light.h"

#define NORMAL 0
#define TOON 1

class Object
{
private:

	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:


	Object(int shaderNum, const string& filename);
	~Object();

	#pragma region メンバー関数

	// 初期化処理　全体で一回のみinitを回す
	static void Init(MyDirectX12* my12);
	static void CreatePiplineStateOBJ();


	void Draw(Light& light);

	void Update();

	void CreateModel(const string& filename);
	void LoadMaterial(const string& directryPath, const string& filename);
	bool LoadTexture(const string& directoryPath, const string& filename);



	// setter
	//void SetModel(Model* model) { this->model = model; }
	void SetShaderNum(int shaderNum) { this->shaderNum = shaderNum; }
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetSize(XMFLOAT3 size) { this->size = size; }
	void SetRadius(float r) { this->r = r; }
	void SetColor(XMFLOAT3 color) { this->color = color; }
	void SetBillboard(bool flg) { isBillboard = flg; }
	void SetBillboardY(bool flg) { isBillboardY = flg; }


	// getter
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetSize() { return size; }
	float GetRadius() { return r; }
	XMFLOAT3 GetObjColor() { return color; }

	static ComPtr<ID3D12Device> GetDevice() { return device; }
	
private:

	// 処理まとめ用

	void SetDrawSetting(int index);
	void DrawCommand(Light& light);
	
	// 定数バッファの生成用
	template<typename T>
	HRESULT CreateConstBuffer(ComPtr<ID3D12Resource>& buffer);

	// バッファの更新用
	template<typename T>
	void UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData);

	#pragma endregion

private:

	#pragma region 構造体

	struct ConstBufferDataB0
	{
		XMMATRIX mat; // 3d変換行列
		XMFLOAT3 cameraPos;
		float pad; // ぺディング
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


	#pragma region	directX用変数

	// 共有する変数
	static ComPtr<ID3D12RootSignature> rootSignature[2]; // ルートシグネチャ
	static ComPtr<ID3D12PipelineState> pipelineState[2]; // パイプラインステート
	//static const int SRVCount = 512; // テクスチャの最大枚数

	static ComPtr<ID3D12GraphicsCommandList> commandList;
	static ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;

	// 定数バッファ数
	static int constBuffNum;


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


	XMMATRIX matWorld; // ワールド行列
	XMMATRIX matScale; // スケーリング行列
	XMMATRIX matRot; // 回転行列
	XMMATRIX matTrans; // 平行移動行列


	XMFLOAT3 position = { 0,0,0 }; // 座標
	XMFLOAT3 scale = { 1,1,1 };     // スケール
	XMFLOAT3 rotation = { 0,0,0 }; // 回転
	XMFLOAT3 color = { 1,1,1 }; // スプライトの色
	UINT texNumber = 0; //	テクスチャ番号

	// どちらかに値を入れてオブジェクトの大きさを取ってくる(主に当たり判定用)
	XMFLOAT3 size = { 0,0,0 }; // 矩形の大きさ
	float r = 0; // 半径

	

	int shaderNum;

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;

	// ビルボード用
	bool isBillboard = false;
	bool isBillboardY = false;
};

template<typename T>
HRESULT Object::CreateConstBuffer(ComPtr<ID3D12Resource>& buffer)
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(T) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);

	return result;
}

template<typename T>
void Object::UpdateBuffer(ComPtr<ID3D12Resource>& buffer,T& bufferData)
{
	T* map = nullptr;
	HRESULT result;

	result = buffer->Map(0, nullptr, (void**)&map);
 	*map = *(&bufferData);
	buffer->Unmap(0, nullptr);
}
