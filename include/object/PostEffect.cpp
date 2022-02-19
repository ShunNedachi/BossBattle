#include "PostEffect.h"
#include"Setting.h"
#include"d3dx12.h"

const float PostEffect::clearColor[4] = { 0.5f,0.5f,0.5f,0.5f };

PostEffect::PostEffect():Sprite2D(0.0f,0.0f)
{
}

void PostEffect::Initialize()
{
	HRESULT result;

	// 初期化処理

	#pragma region 頂点処理

		// 頂点設定
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},	//　左下
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},     // 左上
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}}, // 右下
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},   // 右上
	};
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	//　頂点バッファへのデータ転送
	VertexPosUV* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);


#pragma endregion

	#pragma region テクスチャ
	// テクスチャリソース設定
	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WINDOW_WIDTH,
		(UINT)WINDOW_HEIGHT,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM,clearColor),
		IID_PPV_ARGS(&texBuff));

	assert(SUCCEEDED(result));


	// テクスチャを赤クリア
	// 画素数
	const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	// 画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
	// 画像全体のサイズ
	const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
	// 画像イメージ
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	// テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

	assert(SUCCEEDED(result));
	delete[] img;


#pragma endregion

	#pragma region SRV

	// SRV作成
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	// SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvDescHeap));
	assert(SUCCEEDED(result));

	// SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// デスクリプタヒープにSRV作成
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		srvDescHeap->GetCPUDescriptorHandleForHeapStart());


#pragma endregion

	#pragma region RTV

	// RTV用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	// RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	// デスクリプタヒープにRTV作成
	device->CreateRenderTargetView(texBuff.Get(),
		nullptr,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());

#pragma endregion

	#pragma region DSV

	// 深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	// 深度バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));

	assert(SUCCEEDED(result));

	// DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC depthHeapDesc{};
	depthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthHeapDesc.NumDescriptors = 1;
	// DSV用デスクリプタヒープを作成
	result = device->CreateDescriptorHeap(&depthHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	// デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

#pragma endregion
	

	// constBuff
	//定数バッファの生成
	result = CreateConstBuffer<ConstBufferData>(spriteConstBuff);

	// 定数バッファにデータ転送
	ConstBufferData data;
	// 平行投影行列の合成
	data.mat = DirectX::XMMatrixIdentity();
	data.color = spriteColor;

	UpdateBuffer(spriteConstBuff, data);
}

void PostEffect::Draw()
{
	SetPipelineForSprite();

	using namespace DirectX;

	// 定数バッファの転送
	ConstBufferData data;
	data.mat = XMMatrixIdentity();
	data.color = spriteColor;
	data.alpha = spriteAlpha;
	UpdateBuffer(spriteConstBuff, data);


	DrawCommand();
}

void PostEffect::PreDrawScene()
{
	// リソースバリアを変更(シェーダーリソース->描画可能)
	commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		));

	// レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	// レンダーターゲットをセット
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// ビューポートの設定
	commandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT));
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	// 全画面クリア
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	// 深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDrawScene()
{
	// リソースバリアを変更(描画可能->シェーダーリソース)
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	));

}

void PostEffect::DrawCommand()
{	
	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { srvDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// 定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(1, srvDescHeap->GetGPUDescriptorHandleForHeapStart());

	// 描画コマンド
	commandList->DrawInstanced(4, 1, 0, 0);
}
