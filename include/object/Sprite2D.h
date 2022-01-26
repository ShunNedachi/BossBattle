#pragma once
#include<vector>
#include<DirectXTex.h>
#include<string>
#include"MyWindow.h"
#include"MyDirectX12.h"

struct VertexPosUV
{
	DirectX::XMFLOAT3 pos; // xyz座標
	DirectX::XMFLOAT2 uv; // uv座標
};

struct ConstBufferData
{
	DirectX::XMFLOAT4 color; // 色（RGBA）
	DirectX::XMMATRIX mat; // 3d変換行列
};

class Sprite2D
{
public:
	// アンカーポイントの設定
	Sprite2D(float anchorWidth, float anchorHeigh);
	~Sprite2D();

	// 全体で一度だけ初期化
	static void Init(MyDirectX12* directX);

	static void CreatePipelineStateOBJ(Microsoft::WRL::ComPtr<ID3D12Device> dev);

	// sizeとtexnumberで生成
	HRESULT CreateSprite(UINT texNumber,float sizeX,float sizeY);
	HRESULT CreateSprite(UINT texNumber);


	// 共有テクスチャ番号に画像を記録　拡張子を添える必要あり
	static HRESULT LoadTex(UINT texnumber, const std::string& filename);
	HRESULT Resize(float width, float height);
	HRESULT Resize();


	// 毎フレーム処理
	void SetPipelineForSprite(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList);
	void Draw();

	void InitColor();


	// 座標セット用関数
	void SetPosition(DirectX::XMFLOAT2 position) { spritePosition = position; }
	DirectX::XMFLOAT2 GetPosition() { return spritePosition; }
	// 回転用関数
	void SetRotation(float rotation) { spriteRotation = rotation;}
	float GetRotation() { return spriteRotation; }

	// 頂点バッファ更新用関数
	void UpdateVertices(float width, float height);
	void UpdateVertices();
	// スプライトテクスチャ範囲設定
	void SpriteExtent(float tex_x, float tex_y, float tex_width, float tex_height);


	// 色変更系
	void SetColor(DirectX::XMFLOAT4 color) { spriteColor = color; }
	void SetAlpha(float alpha) { spriteColor.w = alpha; }
	DirectX::XMFLOAT4 GetColor() { return spriteColor; }
	float GetAlpha() { return spriteColor.w; }
	
	// 点滅描画用
	void SetDrawFlash(bool flg) { isDrawFlash = flg; }
	void SetFlashSpeed(float speed) { flashSpeed = speed; }

private:

	// 共有する変数  静的変数
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> spriteRootSignature; // ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> spritePipelineState; // パイプラインステート
	static DirectX::XMMATRIX spriteMatProjection; // 射影行列
	static const int spriteSRVCount = 512; // テクスチャの最大枚数
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> spriteDescHeap;
	static Microsoft::WRL::ComPtr<ID3D12Resource> spriteTexBuff[spriteSRVCount];
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;


	// 1枚ごとに変える変数
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteVertBuff; // 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteConstBuff; // 定数バッファ
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{}; // 頂点バッファビュー

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	float spriteRotation = 0.0f; // ｚ軸周りの回転角
	DirectX::XMFLOAT2 spritePosition{}; // 座標
	DirectX::XMMATRIX spriteMatWorld{}; // ワールド座標
	DirectX::XMFLOAT4 spriteColor = { 1,1,1,1 }; // スプライトの色
	UINT texNumber; //	テクスチャ番号
	DirectX::XMFLOAT2 anchorpoint;
	bool IsFlipX = false;
	bool IsFlipY = false;

	// スプライト切り抜き用
	float tex_x; // テクスチャ左上　ｘ座標
	float tex_y; // テクスチャ左上　ｙ座標
	float tex_width; // テクスチャ横幅
	float tex_height; // テクスチャ縦幅
	bool IsExtent = false;

	// 点滅タイミング用
	bool flashFlag = false;
	// 点滅させるかどうかのフラグ用
	bool isDrawFlash = false;

	float flashSpeed = 1;

	DirectX::XMFLOAT2 originalSize;
};
