#pragma once
#include"MyDirectX12.h"
#include<string>
#include<d3dx12.h>

class Light
{
private:
	// �G�C���A�X
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	// �萔�o�b�t�@�p
	struct ConstBuffLight
	{
		XMFLOAT3 lightPos;
		float pad;
		XMFLOAT3 lightDir;
		float pad2;
		XMFLOAT3 lightColor;
	};

public:

	static void StaticInitialize(ID3D12Device* device);

	// �萔�o�b�t�@�̐����p
	static void Initialize();
	// �萔�o�b�t�@�̍X�V�p
	void Update();

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() { return constBuff->GetGPUVirtualAddress(); }

private:

	// �����܂Ƃߗp

	// �萔�o�b�t�@�̐����p
	template<typename T>
	static HRESULT CreateConstBuffer(ComPtr<ID3D12Resource>& buffer);

	// �o�b�t�@�̍X�V�p
	template<typename T>
	void UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData);


private:

	// �f�o�C�X���
	static ID3D12Device* device;


	// �ʒu�ƕ���
	XMFLOAT3 position = { 0,100,0 };
	XMFLOAT3 dir = { 1,-1,1 };
	// ���C�g�̐F
	XMFLOAT3 color = { 1,1,1 };

	// �萔�o�b�t�@�̏��
	static ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@

};

template<typename T>
HRESULT Light::CreateConstBuffer(ComPtr<ID3D12Resource>& buffer)
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
void Light::UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData)
{
	T* map = nullptr;
	HRESULT result;

	result = buffer->Map(0, nullptr, (void**)&map);
	*map = *(&bufferData);
	buffer->Unmap(0, nullptr);
}
