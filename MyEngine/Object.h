#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<d3dcompiler.h>
#include<Windows.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<d3dx12.h>
#include"MyWindow.h"
#include"MyDirectX12.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


class Object
{
public:
	Object();
	~Object();

	void Init(MyDirectX12* my12, MyWindow* window);
	void CreatePiplineStateOBJ();


	void Draw();

	void Update(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale = { 1,1,1 }, DirectX::XMFLOAT3 rotation = {0,0,0});

	void CreateModel(const std::string &filename);
	void LoadMaterial(const std::string &directryPath, const std::string &filename);
	bool LoadTexture(const std::string &directoryPath, const std::string &filename);

	
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }

	DirectX::XMFLOAT4 GetObjColor() { return objColor; }
	void SetObjColor(DirectX::XMFLOAT4 color) { objColor = color; }
private:

	struct ConstBufferDataB0
	{
		//DirectX::XMFLOAT4 clearColor; // 色（RGBA）
		DirectX::XMMATRIX mat; // 3d変換行列
	};

	// 定数バッファ用データ構造体b1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};
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
		}
	};


	// 共有する変数
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature; // ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState; // パイプラインステート
	static DirectX::XMMATRIX matProjection; // 射影行列
	//static const int SRVCount = 512; // テクスチャの最大枚数
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	static DirectX::XMMATRIX matView;

	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	static MyWindow* window;

	// 1枚ごとに変える変数
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff; // 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff; // インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView{}; // 頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW  ibView{}; // インデックスバッファビュー



	DirectX::XMMATRIX matWorld; // ワールド行列
	DirectX::XMMATRIX matScale; // スケーリング行列
	DirectX::XMMATRIX matRot; // 回転行列
	DirectX::XMMATRIX matTrans; // 平行移動行列


	DirectX::XMFLOAT3 position{}; // 座標
	DirectX::XMFLOAT3 scale{};     // スケール
	DirectX::XMFLOAT3 rotation{}; // 回転
	DirectX::XMMATRIX objMatWorld{}; // ワールド座標
	DirectX::XMFLOAT4 objColor = { 1,1,1,1 }; // スプライトの色
	UINT texNumber =0; //	テクスチャ番号

	

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;

	// eye targetはコンストラクタ内部
};
