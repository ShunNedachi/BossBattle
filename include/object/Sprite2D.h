#pragma once
#include<vector>
#include<DirectXTex.h>
#include<string>
#include"MyWindow.h"
#include"MyDirectX12.h"

struct VertexPosUV
{
	DirectX::XMFLOAT3 pos; // xyz���W
	DirectX::XMFLOAT2 uv; // uv���W
};

struct ConstBufferData
{
	DirectX::XMFLOAT4 color; // �F�iRGBA�j
	DirectX::XMMATRIX mat; // 3d�ϊ��s��
};

class Sprite2D
{
private:
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;

public:
	// �A���J�[�|�C���g�̐ݒ�
	Sprite2D(float anchorWidth, float anchorHeigh);
	~Sprite2D();

	// �S�̂ň�x����������
	static void Init(MyDirectX12* directX);

	static void CreatePipelineStateOBJ(ComPtr<ID3D12Device> dev);

	// size��texnumber�Ő���
	HRESULT CreateSprite(UINT texNumber,float sizeX,float sizeY);
	HRESULT CreateSprite(UINT texNumber);


	// ���L�e�N�X�`���ԍ��ɉ摜���L�^�@�g���q��Y����K�v����
	static HRESULT LoadTex(UINT texnumber, const string& filename);
	HRESULT Resize(float width, float height);
	HRESULT Resize();


	// ���t���[������
	void SetPipelineForSprite(ComPtr<ID3D12GraphicsCommandList> cmdList);
	void Draw();

	void InitColor();


	// ���W�Z�b�g�p�֐�
	void SetPosition(DirectX::XMFLOAT2 position) { spritePosition = position; }
	DirectX::XMFLOAT2 GetPosition() { return spritePosition; }
	// ��]�p�֐�
	void SetRotation(float rotation) { spriteRotation = rotation;}
	float GetRotation() { return spriteRotation; }

	// ���_�o�b�t�@�X�V�p�֐�
	void UpdateVertices(float width, float height);
	void UpdateVertices();
	// �X�v���C�g�e�N�X�`���͈͐ݒ�
	void SpriteExtent(float tex_x, float tex_y, float tex_width, float tex_height);


	// �F�ύX�n
	void SetColor(DirectX::XMFLOAT4 color) { spriteColor = color; }
	void SetAlpha(float alpha) { spriteColor.w = alpha; }
	DirectX::XMFLOAT4 GetColor() { return spriteColor; }
	float GetAlpha() { return spriteColor.w; }
	
	// �_�ŕ`��p
	void SetDrawFlash(bool flg) { isDrawFlash = flg; }
	void SetFlashSpeed(float speed) { flashSpeed = speed; }

private:

	// �����܂Ƃߗp

	void DrawCommand();

	template<typename T>
	HRESULT CreateConstBuffer(ComPtr<ID3D12Resource>& buffer);

	template<typename T>
	void UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData);

private:

	// ���L����ϐ�  �ÓI�ϐ�
	static ComPtr<ID3D12RootSignature> spriteRootSignature; // ���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> spritePipelineState; // �p�C�v���C���X�e�[�g
	static DirectX::XMMATRIX spriteMatProjection; // �ˉe�s��
	static const int spriteSRVCount = 512; // �e�N�X�`���̍ő喇��
	static ComPtr<ID3D12DescriptorHeap> spriteDescHeap;
	static ComPtr<ID3D12Resource> spriteTexBuff[spriteSRVCount];
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12GraphicsCommandList> commandList;


	// 1�����Ƃɕς���ϐ�
	ComPtr<ID3D12Resource> spriteVertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> spriteConstBuff; // �萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{}; // ���_�o�b�t�@�r���[

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	float spriteRotation = 0.0f; // ��������̉�]�p
	DirectX::XMFLOAT2 spritePosition{}; // ���W
	DirectX::XMMATRIX spriteMatWorld{}; // ���[���h���W
	DirectX::XMFLOAT4 spriteColor = { 1,1,1,1 }; // �X�v���C�g�̐F
	UINT texNumber; //	�e�N�X�`���ԍ�
	DirectX::XMFLOAT2 anchorpoint;
	bool IsFlipX = false;
	bool IsFlipY = false;

	// �X�v���C�g�؂蔲���p
	float tex_x; // �e�N�X�`������@�����W
	float tex_y; // �e�N�X�`������@�����W
	float tex_width; // �e�N�X�`������
	float tex_height; // �e�N�X�`���c��
	bool IsExtent = false;

	// �_�Ń^�C�~���O�p
	bool flashFlag = false;
	// �_�ł����邩�ǂ����̃t���O�p
	bool isDrawFlash = false;

	float flashSpeed = 1;

	DirectX::XMFLOAT2 originalSize;
};

template<typename T>
HRESULT Sprite2D::CreateConstBuffer(ComPtr<ID3D12Resource>& buffer)
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
void Sprite2D::UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData)
{
	T* map = nullptr;
	HRESULT result;

	result = buffer->Map(0, nullptr, (void**)&map);
	*map = *(&bufferData);
	buffer->Unmap(0, nullptr);
}
