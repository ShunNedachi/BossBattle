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

	// ����������

	#pragma region ���_����

		// ���_�ݒ�
	VertexPosUV vertices[4] =
	{
		//x y z u v
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},	//�@����
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},     // ����
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},   // �E��
	};
	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff)
	);

	//�@���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);


#pragma endregion

	#pragma region �e�N�X�`��
	// �e�N�X�`�����\�[�X�ݒ�
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


	// �e�N�X�`����ԃN���A
	// ��f��
	const UINT pixelCount = WINDOW_WIDTH * WINDOW_HEIGHT;
	// �摜1�s���̃f�[�^�T�C�Y
	const UINT rowPitch = sizeof(UINT) * WINDOW_WIDTH;
	// �摜�S�̂̃T�C�Y
	const UINT depthPitch = rowPitch * WINDOW_HEIGHT;
	// �摜�C���[�W
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texBuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);

	assert(SUCCEEDED(result));
	delete[] img;


#pragma endregion

	#pragma region SRV

	// SRV�쐬
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&srvDescHeap));
	assert(SUCCEEDED(result));

	// SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// �f�X�N���v�^�q�[�v��SRV�쐬
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		srvDescHeap->GetCPUDescriptorHandleForHeapStart());


#pragma endregion

	#pragma region RTV

	// RTV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	// RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��RTV�쐬
	device->CreateRenderTargetView(texBuff.Get(),
		nullptr,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());

#pragma endregion

	#pragma region DSV

	// �[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	// �[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff));

	assert(SUCCEEDED(result));

	// DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC depthHeapDesc{};
	depthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	depthHeapDesc.NumDescriptors = 1;
	// DSV�p�f�X�N���v�^�q�[�v���쐬
	result = device->CreateDescriptorHeap(&depthHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());

#pragma endregion
	

	// constBuff
	//�萔�o�b�t�@�̐���
	result = CreateConstBuffer<ConstBufferData>(spriteConstBuff);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData data;
	// ���s���e�s��̍���
	data.mat = DirectX::XMMatrixIdentity();
	data.color = spriteColor;

	UpdateBuffer(spriteConstBuff, data);
}

void PostEffect::Draw()
{
	SetPipelineForSprite();

	using namespace DirectX;

	// �萔�o�b�t�@�̓]��
	ConstBufferData data;
	data.mat = XMMatrixIdentity();
	data.color = spriteColor;
	data.alpha = spriteAlpha;
	UpdateBuffer(spriteConstBuff, data);


	DrawCommand();
}

void PostEffect::PreDrawScene()
{
	// ���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X->�`��\)
	commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		));

	// �����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	// �����_�[�^�[�Q�b�g���Z�b�g
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �r���[�|�[�g�̐ݒ�
	commandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT));
	// �V�U�����O��`�̐ݒ�
	commandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

	// �S��ʃN���A
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	// �[�x�o�b�t�@�̃N���A
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::PostDrawScene()
{
	// ���\�[�X�o���A��ύX(�`��\->�V�F�[�_�[���\�[�X)
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	));

}

void PostEffect::DrawCommand()
{	
	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { srvDescHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// ���_�o�b�t�@���Z�b�g
	commandList->IASetVertexBuffers(0, 1, &spriteVBView);

	// �萔�o�b�t�@���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(1, srvDescHeap->GetGPUDescriptorHandleForHeapStart());

	// �`��R�}���h
	commandList->DrawInstanced(4, 1, 0, 0);
}
