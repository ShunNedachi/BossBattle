#include "Light.h"

ID3D12Device* Light::device;
Microsoft::WRL::ComPtr<ID3D12Resource> Light::constBuff;

void Light::StaticInitialize(ID3D12Device* device)
{
	Light::device = device;
	Initialize();
}

void Light::Initialize()
{
	// 定数バッファの生成処理
	CreateConstBuffer<ConstBuffLight>(constBuff);
}

void Light::Update()
{
	// ライトの方向を正規化しておく

	DirectX::XMStoreFloat3(&dir, DirectX::XMVector3Normalize({ dir.x,dir.y,dir.z }));

	ConstBuffLight updateDesc;
	updateDesc.lightPos = position;
	updateDesc.lightDir = dir;
	updateDesc.lightColor = color;
	// 定数バッファの更新処理
	UpdateBuffer(constBuff, updateDesc);
}
