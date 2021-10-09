#pragma once
#include<string>
#include<wrl.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Model
{
private:
	// エイリアス
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using String = std::string;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	
	// 構造体
	struct Vertex
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv; //	uv座標
	};

	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;

		float alpha;
		std::string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;

			textureFilename = "white.png";
		}
	};

	// マテリアル定数バッファ用データ構造体
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};


public:
	 // メンバ関数

	Model() {};
	~Model();

	// 初期化
	static void Initialize(ID3D12Device* device);

	void CreateModel(const String& modelname,int textureNum);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	void CreateBuffer();

	// getter

	ComPtr<ID3D12Resource> GetVertBuff() { return vertBuff; }
	ComPtr<ID3D12Resource> GetIndexBuff() { return indexBuff; }
	ComPtr<ID3D12DescriptorHeap> GetDescHeap() {return descHeap; }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView; }
	D3D12_INDEX_BUFFER_VIEW GetIbView() { return ibView; }
	ComPtr<ID3D12Resource> GetConstBuff() { return constBuff; }
	ComPtr<ID3D12Resource> GetTexBuff() { return texBuff; }

	std::vector<Vertex> GetVertices() { return vertices; }
	std::vector<unsigned short> GetIndices() { return indices; }
	Material GetMaterial() { return material; }

	int GetTexNum() { return texNum; }

private:

	// 静的メンバ変数
	
	static ID3D12Device* device;
	//static UINT descriptorHandleIncrementSize;

	// メンバ変数

	ComPtr<ID3D12DescriptorHeap> descHeap;

	// 頂点バッファ、バッファビュー
	ComPtr<ID3D12Resource> vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファ、バッファビュー
	ComPtr<ID3D12Resource> indexBuff;
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 定数バッファ　マテリアル用
	ComPtr<ID3D12Resource> constBuff;

	// 頂点データ配列
	std::vector<Vertex> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	
	// マテリアル
	Material material;
	ComPtr<ID3D12Resource> texBuff;

	// テクスチャ番号
	UINT texNum = 0;

};

