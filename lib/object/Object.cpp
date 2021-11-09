#include "Object.h"
#include"Setting.h"
#include<vector>
#include<Windows.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<d3dx12.h>
#include<fstream>
#include<sstream>
#include<string>



// 3dObject ���L�ϐ�
Microsoft::WRL::ComPtr<ID3D12RootSignature> Object::rootSignature[2]; // ���[�g�V�O�l�`��
Microsoft::WRL::ComPtr<ID3D12PipelineState> Object::pipelineState[2]; // �p�C�v���C���X�e�[�g
DirectX::XMMATRIX Object::matProjection; // �ˉe�s��
DirectX::XMMATRIX Object::matView;
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Object::commandList;
Microsoft::WRL::ComPtr<ID3D12Device> Object::device;
D3D12_GRAPHICS_PIPELINE_STATE_DESC Object::gpipeline{};
Camera* Object::camera = nullptr;



Object::Object(int shaderNum, const string& filename):shaderNum(shaderNum)
{
	CreateModel(filename);
}
Object::~Object()
{
}

void Object::CreatePiplineStateOBJ()
{
	HRESULT result;
	using namespace Microsoft::WRL;

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	ComPtr<ID3DBlob> vsBlob;// ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;// �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/OBJVertexShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/OBJPixelShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���ݒ�
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

		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{// uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // ���J��Ԃ�
	//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; // �{�[�_�[�̎��͍�
	//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // ��Ԃ��Ȃ�
	//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	//samplerDesc.MinLOD = 0.0f;
	//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // �s�N�Z���V�F�[�_����̂݉�


	// �f�v�X�X�e���V���X�e�[�g
	// �W���I�Ȑݒ�i�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L���j
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	// �W���I�Ȑݒ�i�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

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


	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0���W�X�^

	// ���[�g�p�����[�^ �萔�o�b�t�@�̍���
	//CD3DX12_ROOT_PARAMETER rootparams[2];
	//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL); // �萔�o�b�t�@�Ƃ��ď�����
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	// �o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);

	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[0]));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootSignature[0].Get();

	result = device.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState[0]));


	// �g�D�[���V�F�[�_�[�p���[�g�V�O�l�`���ݒ�
		// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/ToonVertexShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/ToonPixelShader.hlsl", // �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���ݒ�
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
	D3D12_INPUT_ELEMENT_DESC inputLayout2[] =
	{
		{// xyz���W(1�s�ŏ������ق������₷��)
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{// uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.InputLayout.pInputElementDescs = inputLayout2;
	gpipeline.InputLayout.NumElements = _countof(inputLayout2);

	// ���[�g�V�O�l�`���̐���
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[1]));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootSignature[1].Get();

	result = device.Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState[1]));




}

void Object::Draw()
{
	// num �ɂ���ăV�F�[�_�[��؂�ւ���
	if (shaderNum == 0)
	{	
		// �p�C�v���C���X�e�[�g�̐ݒ�
		commandList->SetPipelineState(pipelineState[0].Get());
		// ���[�g�V�O�l�`���̐ݒ�
		commandList->SetGraphicsRootSignature(rootSignature[0].Get());
		// �v���~�e�B�u�`���ݒ�
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	else if (shaderNum == 1)
	{
		// �p�C�v���C���X�e�[�g�̐ݒ�
		commandList->SetPipelineState(pipelineState[1].Get());
		// ���[�g�V�O�l�`���̐ݒ�
		commandList->SetGraphicsRootSignature(rootSignature[1].Get());
		// �v���~�e�B�u�`���ݒ�
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	}

	using namespace DirectX;
	HRESULT result;

	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matView * matProjection;
	constMap->cameraPos = camera->GetEye();
	constMap->color = color;
	//constMap->colorR = color.x;
	//constMap->colorG = color.y;
	//constMap->colorB = color.z;
	//constMap->colorA = color.w;
	constBuffB0->Unmap(0, nullptr);

	// b1�f�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);


	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


	// ���_�o�b�t�@���Z�b�g
	commandList->IASetVertexBuffers(0, 1, &vbView);

	commandList->IASetIndexBuffer(&ibView);

	// �萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	// �}�e���A���֌W
	commandList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeap->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));


	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if(vertices.size() ==0)commandList->DrawIndexedInstanced(18, 1, 0, 0, 0);
	else commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}


void Object::Update()
{
	using namespace DirectX;
	HRESULT result;

	// �s��̍X�V

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	// �r���[�s��
	matView = XMMatrixLookAtLH(XMLoadFloat3(&camera->GetEye()), XMLoadFloat3(&camera->GetTarget()), XMLoadFloat3(&camera->GetUp()));

}

void Object::Init(MyDirectX12* my12)
{
	using namespace DirectX;


	// �ˉe�ϊ�
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WINDOW_WIDTH / WINDOW_HEIGHT,
		0.1f, 1000.0f
	);




	commandList = my12->CommandList();
	device = my12->Device();
	Object::camera = Camera::GetInstance();
	CreatePiplineStateOBJ();
}

void Object::CreateModel(const string &modelname)
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));


	// �t�@�C���X�g���[��
	std::ifstream file;
	// obj�t�@�C�����J��
	//file.open("Resources/object/triangle/triangle.obj");

	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/object/" + modelname + "/";
	file.open(directoryPath + filename);

	if (file.fail())assert(0);

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	// �l�p�`�|���S���p
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
			while (getline(line_stream,index_string,' '))
			{

				std::istringstream index_stream(index_string);
				unsigned short indexPosition,indexNormal,indexTexcoord;
				index_stream >> indexPosition;

				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				
				if (faceCount >= 3)
				{
					// �l�p�`�|���S����4�_��
					// 012�ł��łɍ���Ă���̂�230�ŎO�p�����
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


	UINT sizeVB = static_cast<UINT>(sizeof(Vertex)*vertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());

	// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);


	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);


	// 		
	//�@���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	//memcpy(vertMap, vertices, sizeof(vertices));
	std::copy(vertices.begin(), vertices.end(), vertMap);
	vertBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//for (int i = 0; i < _countof(indices); i++)
	//{
	//	indexMap[i] = indices[i]; // �C���f�b�N�X���R�s�[
	//}
	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);


	//// �e�N�X�`�����擾
	//D3D12_RESOURCE_DESC resDesc =
	//	texBuff->GetDesc();

	////�@�g�p����e�N�X�`���ԍ��������o�ϐ��ɓ����
	//this->texNumber = texNumber;

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);
	// �C���f�b�N�X�o�b�t�@�r���[�̐���
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


	//�萔�o�b�t�@�̐��� b0
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);

	// b1
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);

	// b0 �f�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matView * matProjection;
	//constMap->cameraPos = camera->GetEye();
	constMap->color = color;
	//constMap->colorR = color.x;
	//constMap->colorG = color.y;
	//constMap->colorB = color.z;
	//constMap->colorA = color.w;

	constBuffB0->Unmap(0, nullptr);

	// b1�f�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);

}

void Object::LoadMaterial(const std::string &directoryPath, const std::string &filename)
{

	std::ifstream file;
	bool isTexture = false;

	file.open(directoryPath + filename);

	if (file.fail())assert(0);

	string line;
	while (getline(file,line))
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
	if(!isTexture)LoadTexture(TEXT("Resources/texture/"), "white.png");

	file.close();
}

bool Object::LoadTexture(const std::string &directoryPath, const std::string &filename)
{
	using namespace DirectX;
	HRESULT result;
	// �e�N�X�`���n
	// WIC�e�N�X�`���̃��[�h
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
		IID_PPV_ARGS(&texBuff)
	);

	result = texBuff->WriteToSubresource(
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
		texBuff.Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeap->GetCPUDescriptorHandleForHeapStart(),
			texNumber,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);


	return S_OK;

}
