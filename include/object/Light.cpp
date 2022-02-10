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
	// �萔�o�b�t�@�̐�������
	CreateConstBuffer<ConstBuffLight>(constBuff);
}

void Light::Update()
{
	ConstBuffLight updateDesc;
	updateDesc.lightPos = position;
	updateDesc.lightDir = dir;
	updateDesc.lightColor = color;
	// �萔�o�b�t�@�̍X�V����
	UpdateBuffer(constBuff, updateDesc);
}
