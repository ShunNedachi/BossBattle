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
public:
	// �A���J�[�|�C���g�̐ݒ�
	Sprite2D(float anchorWidth, float anchorHeigh);
	~Sprite2D();

	// �S�̂ň�x����������
	static void Init(MyDirectX12* directX);

	static void CreatePipelineStateOBJ(Microsoft::WRL::ComPtr<ID3D12Device> dev);

	// size��texnumber�Ő���
	HRESULT CreateSprite(UINT texNumber,float sizeX,float sizeY);
	HRESULT CreateSprite(UINT texNumber);


	// ���L�e�N�X�`���ԍ��ɉ摜���L�^�@�g���q��Y����K�v����
	static HRESULT LoadTex(UINT texnumber, const std::string& filename);
	HRESULT Resize(float width, float height);
	HRESULT Resize();


	// ���t���[������
	void SetPipelineForSprite(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList);
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

	// ���L����ϐ�  �ÓI�ϐ�
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> spriteRootSignature; // ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> spritePipelineState; // �p�C�v���C���X�e�[�g
	static DirectX::XMMATRIX spriteMatProjection; // �ˉe�s��
	static const int spriteSRVCount = 512; // �e�N�X�`���̍ő喇��
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> spriteDescHeap;
	static Microsoft::WRL::ComPtr<ID3D12Resource> spriteTexBuff[spriteSRVCount];
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;


	// 1�����Ƃɕς���ϐ�
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteVertBuff; // ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteConstBuff; // �萔�o�b�t�@
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
