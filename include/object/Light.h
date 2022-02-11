#pragma once
#include"MyDirectX12.h"
#include<string>
#include<d3dx12.h>

class Light
{
private:
	// エイリアス
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	// 定数バッファ用
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

	// 定数バッファの生成用
	static void Initialize();
	// 定数バッファの更新用
	void Update();

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() { return constBuff->GetGPUVirtualAddress(); }

private:

	// 処理まとめ用

	// 定数バッファの生成用
	template<typename T>
	static HRESULT CreateConstBuffer(ComPtr<ID3D12Resource>& buffer);

	// バッファの更新用
	template<typename T>
	void UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData);


private:

	// デバイス情報
	static ID3D12Device* device;


	// 位置と方向
	XMFLOAT3 position = { 0,100,0 };
	XMFLOAT3 dir = { 1,-1,1 };
	// ライトの色
	XMFLOAT3 color = { 1,1,1 };

	// 定数バッファの情報
	static ComPtr<ID3D12Resource> constBuff; // 定数バッファ

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
