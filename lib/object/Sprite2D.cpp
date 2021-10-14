#include "Sprite2D.h"
#include<vector>
#include<wrl.h>
#include<d3dx12.h>


using namespace DirectX;
using namespace Microsoft::WRL;


//// ���L����ϐ�
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
	//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // ���J��Ԃ�
	//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // �{�[�_�[�̎��͍�
	//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // ��Ԃ��Ȃ�
	//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	//samplerDesc.MinLOD = 0.0f;
	//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // �s�N�Z���V�F�[�_����̂݉�


	ComPtr<ID3DBlob> vsBlob;// ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;// �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/SpriteVertexShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/SpritePixelShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���ݒ�
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result))
	{

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{// xyz���W(1�s�ŏ������ق������₷��)
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},


		{// uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};


	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�X�e�[�g
	// �W���I�Ȑݒ�i�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L���j
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʃJ�����O���Ȃ�

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	// �W���I�Ȑݒ�i�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�i�W���邪���͂P�����g��Ȃ��j
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // �W���ݒ�

			// �u�����h�̋��ʐݒ�

	blenddesc.BlendEnable = true; // �u�����h�L��
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; // �\�[�X�̒l���P�O�O���g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; // �f�X�g�̒l���O%�g��


	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // ���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; // �\�[�X�̒l�̃��l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f�\�[�X�̃��l
	// 


	gpipeline.BlendState.RenderTarget[0] = blenddesc; // RBGA�S�Ẵ`�����l����`��


	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ͂P��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ��P��T���v�����O


	//�@���[�g�V�O�l�`���̐���

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL); // �萔�o�b�t�@�Ƃ��ď�����
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	// �o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	// ���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = spriteRootSignature.Get();

	result = dev.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelineState));


	// �f�X�N���v�^�q�[�v�𐶐�
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

	// �e�N�X�`���ԍ����L�^
	this->texNumber = texNumber;


	// ���_�f�[�^
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//�@����
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // ����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // �E��
	};


	// �����o�ϐ�������
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f
	);

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };
	// �e�N�X�`�����擾
	//D3D12_RESOURCE_DESC resDesc =
	//	spriteTexBuff[texNumber]->GetDesc();

	//�@�g�p����e�N�X�`���ԍ��������o�ϐ��ɓ����
	//this->texNumber = texNumber;

	UpdateVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff)
	);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f); // ���s���e�s��̍���
	spriteConstBuff->Unmap(0, nullptr);

	Resize(sizeX, sizeY);

	return result;
}
HRESULT Sprite2D::CreateSprite(UINT texNumber)
{
	using namespace DirectX;
	HRESULT result = S_FALSE;

	// �e�N�X�`���ԍ����L�^
	this->texNumber = texNumber;


	// ���_�f�[�^
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//�@����
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // ����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // �E��
	};


	// �����o�ϐ�������
	spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f
	);

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };
	// �e�N�X�`�����擾
	//D3D12_RESOURCE_DESC resDesc =
	//	spriteTexBuff[texNumber]->GetDesc();

	//�@�g�p����e�N�X�`���ԍ��������o�ϐ��ɓ����
	//this->texNumber = texNumber;

	UpdateVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff)
	);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window->GetWidth(), window->GetHeight(), 0.0f, 0.0f, 1.0f); // ���s���e�s��̍���
	spriteConstBuff->Unmap(0, nullptr);

	Resize();

	return result;
}


// �g���q��Y����K�v����
HRESULT Sprite2D::LoadTex(UINT texnumber, const std::string& filename)
{
	using namespace DirectX;
	HRESULT result;

	std::string fullPath = "Resources/texture/" + filename;

	// string��ϊ�
	WCHAR wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(),
		-1, wfilepath, _countof(wfilepath));

	// �e�N�X�`���n
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfilepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0);


	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�̃o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,  // �e�N�X�`���p�w��
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


	// �V�F�[�_�[���\�[�X�r���[�쐬

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
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
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spritePipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite2D::Draw()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	commandList->SetPipelineState(spritePipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	commandList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	using namespace DirectX;

	// ���[���h�s��̍X�V
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y,0);

	// �s��̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection; // �s��̍���
	constMap->color = spriteColor;
	spriteConstBuff->Unmap(0, nullptr);



	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// ���_�o�b�t�@���Z�b�g
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// �萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
	// �`��R�}���h
	commandList->DrawInstanced(4, 1, 0, 0);
}
void Sprite2D::DrawFlash()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	commandList->SetPipelineState(spritePipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	commandList->SetGraphicsRootSignature(spriteRootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	using namespace DirectX;

	// ���[���h�s��̍X�V
	spriteMatWorld = XMMatrixIdentity();
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spriteMatWorld *= XMMatrixTranslation(spritePosition.x, spritePosition.y, 0);

	// �s��̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = spriteMatWorld * spriteMatProjection; // �s��̍���
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

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// ���_�o�b�t�@���Z�b�g
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// �萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
	// �`��R�}���h
	commandList->DrawInstanced(4, 1, 0, 0);
}

void Sprite2D::InitColor()
{
	spriteColor = { 1,1,1,1 };
	// �s��̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = spriteColor;
	spriteConstBuff->Unmap(0, nullptr);

}

// �\���T�C�Y�ݒ�
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


// �T�C�Y�ݒ�p
void Sprite2D::UpdateVertices(float width, float height)
{
	HRESULT result;

	// ���_�f�[�^
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//�@����
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // ����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * width;
	float right = (1.0f - anchorpoint.x)* width;
	float top = (0.0f - anchorpoint.y)* height;
	float bottom = (1.0f - anchorpoint.y)* height;

	if (IsFlipX)
	{
		// ���E����ւ�
		left = -left;
		right = -right;
	}
	if (IsFlipY)
	{
		// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = { left,bottom,0.0f }; // ����
	vertices[LT].pos = { left ,top,0.0f }; // ����
	vertices[RB].pos = { right,bottom,0.0f }; // �E��
	vertices[RT].pos = { right,top,0.0f }; // �E��



	if (spriteTexBuff[texNumber] && IsExtent)
	{
		D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom }; // ����
		vertices[LT].uv = { tex_left, tex_top }; // ����
		vertices[RB].uv = { tex_right,tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,tex_top }; // �E��

		IsExtent = false;
	}

	//�@���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);
}

void Sprite2D::UpdateVertices()
{
	HRESULT result;

	// ���_�f�[�^
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},	//�@����
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},     // ����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},   // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	D3D12_RESOURCE_DESC resDesc = spriteTexBuff[texNumber]->GetDesc();
	float resWidth = resDesc.Width; // �摜�̉���
	float resHeight = resDesc.Height; // �摜�̏c��


	float left = (0.0f - anchorpoint.x) * resWidth;
	float right = (1.0f - anchorpoint.x)* resWidth;
	float top = (0.0f - anchorpoint.y)*resHeight;
	float bottom = (1.0f - anchorpoint.y)*resHeight;

	if (IsFlipX)
	{
		// ���E����ւ�
		left = -left;
		right = -right;
	}
	if (IsFlipY)
	{
		// �㉺����ւ�
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = { left,bottom,0.0f }; // ����
	vertices[LT].pos = { left ,top,0.0f }; // ����
	vertices[RB].pos = { right,bottom,0.0f }; // �E��
	vertices[RT].pos = { right,top,0.0f }; // �E��


	if (spriteTexBuff[texNumber] && IsExtent)
	{
		float tex_left = tex_x / resDesc.Width;
		float tex_right = (tex_x + tex_width) / resDesc.Width;
		float tex_top = tex_y / resDesc.Height;
		float tex_bottom = (tex_y + tex_height) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom }; // ����
		vertices[LT].uv = { tex_left, tex_top }; // ����
		vertices[RB].uv = { tex_right,tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,tex_top }; // �E��

		IsExtent = false;
	}


	//�@���_�o�b�t�@�ւ̃f�[�^�]��
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
	// ���L�ϐ��̋L�^
	Sprite2D::device = directX->Device();
	Sprite2D::commandList = directX->CommandList();
	Sprite2D::window = window;

	CreatePipelineStateOBJ(device);
}