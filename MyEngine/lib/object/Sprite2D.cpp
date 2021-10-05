#include "Sprite2D.h"
#include<vector>
#include<wrl.h>
#include<d3dx12.h>


using namespace DirectX;
using namespace Microsoft::WRL;


//// 共有する変数
ComPtr<ID3D12RootSignature> Sprite2D::spriteRootSignature = nullptr;
ComPtr<ID3D12PipelineState> Sprite2D::spritePipelineState = nullptr;
DirectX::XMMATRIX Sprite2D::spriteMatProjection = DirectX::XMMatrixIdentity();
ComPtr<ID3D12DescriptorHeap> Sprite2D::spriteDescHeap = nullptr;
ComPtr<ID3D12Resource> Sprite2D::spriteTexBuff[spriteSRVCount] = {};
ComPtr<ID3D12Device> Sprite2D::device;
ComPtr<ID3D12GraphicsCommandList> Sprite2D::commandList;
MyWindow* Sprite2D::window;


Sprite2D::Sprite2D(float anchorWidth, float anchorHeight) :anchorpoint{ anchorWidth,anchorHeight }
{
}
Sprite2D::~Sprite2D()
{
}

void Sprite2D::CreatePipelineStateOBJ(Microsoft::WRL::ComPtr<ID3D12Device> dev)
{
	HRESULT result;
	using namespace Microsoft::WRL;

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 横繰り返し
	//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // ボーダーの時は黒
	//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // 補間しない
	//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	//samplerDesc.MinLOD = 0.0f;
	//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダからのみ可視


	ComPtr<ID3DBlob> vsBlob;// 頂点シェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;// ピクセルシェーダーオブジェクト
	ComPtr<ID3DBlob> errorBlob;// エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/SpriteVertexShader.hlsl", // シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"VSmain", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/SpritePixelShader.hlsl", // シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"PSmain", "ps_5_0", // エントリーポイント名、シェーダーモデル設定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result))
	{

		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{// xyz座標(1行で書いたほうが見やすい)
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},


		{// uv座標(1行で書いたほうが見やすい)
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};


	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// ラスタライザステート
	// 標準的な設定（背面カリング、塗りつぶし、深度クリッピング有効）
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // 背面カリングしない

	// デプスステンシルステートの設定
	// 標準的な設定（深度テストを行う、書き込み許可、深度が小さければ合格）
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定（８個あるが今は１つしか使わない）
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 標準設定

			// ブレンドの共通設定

	blenddesc.BlendEnable = true; // ブレンド有効
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; // ソースの値を１００％使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストの値を０%使う


	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; // ソースの値のα値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0fソースのα値
	// 


	gpipeline.BlendState.RenderTarget[0] = blenddesc; // RBGA全てのチャンネルを描画


	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は１つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき１回サンプリング


	//　ルートシグネチャの生成

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL); // 定数バッファとして初期化
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	// バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	// ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = spriteRootSignature.Get();

	result = dev.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelineState));


	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));

}

HRESULT Sprite2D::CreateSprite(UINT texNumber, float sizeX, float sizeY)
{
	using namespace DirectX;
	HRESULT result = S_FALSE;

	// テクスチャ番号を記録
	this->texNumber = texNumber;


	// 頂点データ
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//　左下
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // 左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // 右上
	};


	// メンバ変数初期化
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f
	);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };
	// テクスチャ情報取得
	//D3D12_RESOURCE_DESC resDesc =
	//	spriteTexBuff[texNumber]->GetDesc();

	//　使用するテクスチャ番号をメンバ変数に入れる
	//this->texNumber = texNumber;

	UpdateVertices();

	// 頂点バッファビューの作成
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff)
	);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f); // 平行投影行列の合成
	spriteConstBuff->Unmap(0, nullptr);

	Resize(sizeX, sizeY);

	return result;
}
HRESULT Sprite2D::CreateSprite(UINT texNumber)
{
	using namespace DirectX;
	HRESULT result = S_FALSE;

	// テクスチャ番号を記録
	this->texNumber = texNumber;


	// 頂点データ
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//　左下
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // 左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // 右上
	};


	// メンバ変数初期化
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f
	);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };
	// テクスチャ情報取得
	//D3D12_RESOURCE_DESC resDesc =
	//	spriteTexBuff[texNumber]->GetDesc();

	//　使用するテクスチャ番号をメンバ変数に入れる
	//this->texNumber = texNumber;

	UpdateVertices();

	// 頂点バッファビューの作成
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff)
	);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f); // 平行投影行列の合成
	spriteConstBuff->Unmap(0, nullptr);

	Resize();

	return result;
}


// 拡張子を添える必要あり
HRESULT Sprite2D::LoadTex(UINT texnumber, const std::string& filename)
{
	using namespace DirectX;
	HRESULT result;

	std::string fullPath = "Resources/texture/" + filename;

	// stringを変換
	WCHAR wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(),
		-1, wfilepath, _countof(wfilepath));

	// テクスチャ系
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);


	// リソース設定
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用のバッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,  // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&spriteTexBuff[texnumber])
	);

	result = spriteTexBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);


	// シェーダーリソースビュー作成

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		spriteTexBuff[texnumber].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetCPUDescriptorHandleForHeapStart(),
			texnumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);

	return S_OK;
}

void Sprite2D::SetPipelineForSprite(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList)
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(spritePipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite2D::Draw()
{
	// パイプラインステートの設定
	commandList->SetPipelineState(spritePipelineState.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	using namespace DirectX;

	// ワールド行列の更新
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y,0);

	// 行列の転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection; // 行列の合成
	constMap->color = spriteColor;
	spriteConstBuff->Unmap(0, nullptr);



	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// 定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
	// 描画コマンド
	commandList->DrawInstanced(4, 1, 0, 0);
}
void Sprite2D::DrawFlash()
{
	// パイプラインステートの設定
	commandList->SetPipelineState(spritePipelineState.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	using namespace DirectX;

	// ワールド行列の更新
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y, 0);

	// 行列の転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection; // 行列の合成
	constMap->color = spriteColor;
	spriteConstBuff->Unmap(0, nullptr);

	if (spriteColor.x >= 0.9)
	{
		flashFlag = true;
	}
	else if (spriteColor.x <= 0)
	{
		flashFlag = false;
	}

	if (flashFlag)
	{
		spriteColor.x -= 0.01f;
		spriteColor.y -= 0.01f;
		spriteColor.z -= 0.01f;
		spriteColor.w -= 0.01f;
	}
	else
	{
		spriteColor.x += 0.01f;
		spriteColor.y += 0.01f;
		spriteColor.z += 0.01f;
		spriteColor.w += 0.01f;
	}

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// 定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
	// 描画コマンド
	commandList->DrawInstanced(4, 1, 0, 0);
}

void Sprite2D::InitColor()
{
	spriteColor = { 1,1,1,1 };
	// 行列の転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = spriteColor;
	spriteConstBuff->Unmap(0, nullptr);

}

// 表示サイズ設定
HRESULT Sprite2D::Resize(float width, float height)
{

	UpdateVertices(width, height);

	return S_OK;
}

HRESULT Sprite2D::Resize()
{

	UpdateVertices();

	return S_OK;
}


// サイズ設定用
void Sprite2D::UpdateVertices(float width, float height)
{
	HRESULT result;

	// 頂点データ
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//　左下
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // 左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * width;
	float right = (1.0f - anchorpoint.x)* width;
	float top = (0.0f - anchorpoint.y)* height;
	float bottom = (1.0f - anchorpoint.y)* height;

	if (IsFlipX)
	{
		// 左右入れ替え
		left = -left;
		right = -right;
	}
	if (IsFlipY)
	{
		// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = { left,bottom,0.0f }; // 左下
	vertices[LT].pos = { left ,top,0.0f }; // 左上
	vertices[RB].pos = { right,bottom,0.0f }; // 右下
	vertices[RT].pos = { right,top,0.0f }; // 右上



	if (spriteTexBuff[texNumber] && IsExtent)
	{
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom }; // 左下
		vertices[LT].uv = { tex_left, tex_top }; // 左上
		vertices[RB].uv = { tex_right,tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,tex_top }; // 右上

		IsExtent = false;
	}

	//　頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);
}

void Sprite2D::UpdateVertices()
{
	HRESULT result;

	// 頂点データ
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//　左下
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // 左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
	float resWidth = resDesc.Width; // 画像の横幅
	float resHeight = resDesc.Height; // 画像の縦幅


	float left = (0.0f - anchorpoint.x) * resWidth;
	float right = (1.0f - anchorpoint.x)* resWidth;
	float top = (0.0f - anchorpoint.y)*resHeight;
	float bottom = (1.0f - anchorpoint.y)*resHeight;

	if (IsFlipX)
	{
		// 左右入れ替え
		left = -left;
		right = -right;
	}
	if (IsFlipY)
	{
		// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = { left,bottom,0.0f }; // 左下
	vertices[LT].pos = { left ,top,0.0f }; // 左上
	vertices[RB].pos = { right,bottom,0.0f }; // 右下
	vertices[RT].pos = { right,top,0.0f }; // 右上


	if (spriteTexBuff[texNumber] && IsExtent)
	{
		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom }; // 左下
		vertices[LT].uv = { tex_left, tex_top }; // 左上
		vertices[RB].uv = { tex_right,tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,tex_top }; // 右上

		IsExtent = false;
	}


	//　頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);
}

void Sprite2D::SpriteExtent(float tex_x, float tex_y, float tex_width, float tex_height)
{
	this->tex_x = tex_x;
	this->tex_y = tex_y;
	this->tex_width = tex_width;
	this->tex_height = tex_height;

	IsExtent = true;
	UpdateVertices();
}


void Sprite2D::Init(MyDirectX12* directX,MyWindow* window)
{
	// 共有変数の記録
	Sprite2D::device = directX->Device();
	Sprite2D::commandList = directX->CommandList();
	Sprite2D::window = window;

	CreatePipelineStateOBJ(device);
}