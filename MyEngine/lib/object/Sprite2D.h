#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<d3dcompiler.h>
#include<Windows.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<d3dx12.h>
#include"MyWindow.h"
#include"MyDirectX12.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

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

	static void CreatePipelineStateOBJ(Microsoft::WRL::ComPtr<ID3D12Device> dev);

	// size��texnumber�Ő���
	HRESULT CreateSprite(UINT texNumber,float sizeX,float sizeY);
	HRESULT CreateSprite(UINT texNumber);


	// ���L�e�N�X�`���ԍ��ɉ摜���L�^
	static HRESULT LoadTex(UINT texnumber, const wchar_t* filename);
	HRESULT Resize(float width, float height);
	HRESULT Resize();


	// ���t���[������
	void SetPipelineForSprite(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList);
	void Draw();
	void DrawFlash();

	void InitColor();


	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC Getgpipeline();

	// ���W�Z�b�g�p�֐�
	void SetPosition(DirectX::XMFLOAT2 position);
	// ���_�o�b�t�@�X�V�p�֐�
	void UpdateVertices(float width, float height);
	void UpdateVertices();
	// �X�v���C�g�e�N�X�`���͈͐ݒ�
	void SpriteExtent(float tex_x, float tex_y, float tex_width, float tex_height);

	// �S�̂ň�x����������
	static void Init(MyDirectX12* directX,MyWindow* window);
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
	static MyWindow* window;


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

	bool flashFlag = false;
};
