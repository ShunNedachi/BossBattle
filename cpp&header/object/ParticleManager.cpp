#include "ParticleManager.h"
#include<fstream>
#include<sstream>
#include<string>

Microsoft::WRL::ComPtr<ID3D12RootSignature> ParticleManager::rootSignature; // ルートシグネチャ
Microsoft::WRL::ComPtr<ID3D12PipelineState> ParticleManager::pipelineState; // パイプラインステート
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> ParticleManager::commandList;
Microsoft::WRL::ComPtr<ID3D12Device> ParticleManager::device;
Microsoft::WRL::ComPtr<ID3D12Resource> ParticleManager::texBuff;
D3D12_GRAPHICS_PIPELINE_STATE_DESC ParticleManager::gpipeline;
int ParticleManager::constBuffNum = 2; // b0,b1

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Init(MyDirectX12* my12)
{
	commandList = my12->CommandList();
	device = my12->Device();
	CreatePiplineStateOBJ();
}

void ParticleManager::CreatePiplineStateOBJ()
{
	HRESULT result;
	using namespace Microsoft::WRL;

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	#pragma region 通常シェーダー読み込み

	ComPtr<ID3DBlob> vsBlob;// 頂点シェーダーオブジェクト
	ComPtr<ID3DBlob> psBlob;// ピクセルシェーダーオブジェクト
	ComPtr<ID3DBlob> errorBlob;// エラーオブジェクト

	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/ParticleVS.hlsl", // シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"VSmain", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/ParticlePS.hlsl", // シェーダーファイル名
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

		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{// uv座標(1行で書いたほうが見やすい)
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());



#pragma endregion

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// デプスステンシルステート
	// 標準的な設定（背面カリング、塗りつぶし、深度クリッピング有効）
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// デプスステンシルステートの設定
	// 標準的な設定（深度テストを行う、書き込み許可、深度が小さければ合格）
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

#pragma region ブレンド関係

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

#pragma endregion

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は１つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき１回サンプリング


	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0レジスタ

	// ルートパラメータの生成
	std::vector<CD3DX12_ROOT_PARAMETER> rootparams;
	for (int i = 0; i < constBuffNum; i++)
	{
		rootparams.emplace_back();
		rootparams.at(i).InitAsConstantBufferView(i, 0, D3D12_SHADER_VISIBILITY_ALL);
	}
	rootparams.emplace_back();
	rootparams.back().InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(rootparams.size(), &rootparams[0], 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	// バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootSignature.Get();

	result = device.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
}

void ParticleManager::Draw()
{
	// シェーダーを切り替える
	SetDrawSetting();

	using namespace DirectX;
	HRESULT result;

	XMMATRIX matView = Camera::GetViewMatrix();
	XMMATRIX matProjection = Camera::GetProjection();

	XMMATRIX spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= XMMatrixTranslation(0,0,0);

	// b0データ転送
	ConstBufferDataB0 dataB0;
	dataB0.mat = spriteMatWorld;
	dataB0.cameraPos = Camera::GetInstance()->GetEye();
	dataB0.color = { 1,1,1,1 };
	// ビルボードをかける
	if (isBillboard) { dataB0.billboardMat = Camera::GetBillboardMatrix(); }
	else if (isBillboardY) { dataB0.billboardMat = Camera::GetBillboardYMatrix(); }
	else { dataB0.billboardMat = XMMatrixIdentity(); }
	// 転送
	UpdateBuffer(constBuffB0, dataB0);

	// b1データ転送
	ConstBufferDataB1 dataB1;
	dataB1.ambient = material.ambient;
	dataB1.diffuse = material.diffuse;
	dataB1.specular = material.specular;
	dataB1.alpha = material.alpha;

	UpdateBuffer(constBuffB1, dataB1);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	DrawCommand();

}

void ParticleManager::Update()
{
}

void ParticleManager::CreateModel()
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));

#pragma region file読み込み処理

	// ファイルストリーム
	std::ifstream file;
	// objファイルを開く
	//file.open("Resources/object/triangle/triangle.obj");

	const string filename = "player.obj";
	const string directoryPath = "Resources/object/player/";
	file.open(directoryPath + filename);

	if (file.fail())assert(0);

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	// 四角形ポリゴン用
	int indexCount = 0;

	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.emplace_back(position);

			//Vertex vertex{};
			//vertex.pos = position;
			//vertices.emplace_back(vertex);
		}
		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}
		if (key == "f")
		{
			int faceCount = 0;

			string index_string;
			while (getline(line_stream, index_string, ' '))
			{

				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;

				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);


				if (faceCount >= 3)
				{
					// 四角形ポリゴンの4点目
					// 012ですでに作っているので230で三角を作る
					indices.emplace_back(indexCount - 1);
					indices.emplace_back(indexCount);
					indices.emplace_back(indexCount - 3);

				}
				else
				{

					indices.emplace_back(indexCount);
				}

				faceCount++;
				indexCount++;
			}


		}
		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;

			LoadMaterial(directoryPath, filename);
		}
	}
	file.close();

#pragma endregion

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	// 頂点バッファ、インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);


	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);



	// 		
	//　頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);


	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
	// インデックスバッファビューの生成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


	//定数バッファの生成 b0
	result = CreateConstBuffer<ConstBufferDataB0>(constBuffB0);
	// b1
	result = CreateConstBuffer<ConstBufferDataB1>(constBuffB1);

	XMMATRIX matView = Camera::GetViewMatrix();
	XMMATRIX matProjection = Camera::GetProjection();

	//// b0 データ転送用　変数定義
	//ConstBufferDataB0 dataB0;
	//// 転送
	//UpdateBuffer(constBuffB0, dataB0);

	//// b1データ転送用　変数定義
	//ConstBufferDataB1 dataB1;
	//dataB1.ambient = material.ambient;
	//dataB1.diffuse = material.diffuse;
	//dataB1.specular = material.specular;
	//dataB1.alpha = material.alpha;
	//UpdateBuffer(constBuffB1, dataB1);

}

void ParticleManager::LoadMaterial(const string& directoryPath, const string& filename)
{
	std::ifstream file;
	bool isTexture = false;

	file.open(directoryPath + filename);

	if (file.fail())assert(0);

	string line;
	while (getline(file, line))
	{

		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_stream >> material.name;
		}
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> material.textureFilename;

			LoadTexture(directoryPath, material.textureFilename);
			isTexture = true;

		}
	}
	if (!isTexture)LoadTexture(TEXT("Resources/texture/"), "white.png");

	file.close();

}

bool ParticleManager::LoadTexture(const string& directoryPath, const string& filename)
{
	using namespace DirectX;
	HRESULT result;
	// テクスチャ系
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};


	string filepath = directoryPath + filename;

	WCHAR wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(),
		-1, wfilepath, _countof(wfilepath));

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
		IID_PPV_ARGS(&texBuff)
	);

	result = texBuff->WriteToSubresource(
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
		texBuff.Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);


	return S_OK;

}

void ParticleManager::SetDrawSetting()
{
	// パイプラインステートの設定
	commandList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void ParticleManager::DrawCommand()
{
	// 頂点バッファをセット
	commandList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	commandList->IASetIndexBuffer(&ibView);


	// 定数バッファをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(constBuffNum,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));


	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// バグった場合に、object.cppを参考にしなおす
	commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);

}
