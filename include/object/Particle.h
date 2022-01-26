#pragma once
#include<vector>
#include<DirectXTex.h>
#include"MyDirectX12.h"
#include"Camera.h"
#include<string>
#include"Model.h"


#define NORMAL 0
#define TOON 1
class Particle
{
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:


	Particle();
	~Particle();

#pragma region �����o�[�֐�

	// �����������@�S�̂ň��̂�init����
	static void Init(MyDirectX12* my12);
	static void CreatePiplineStateOBJ();


	void Draw();
	void DrawFlash();

	void Update();

	void CreateModel();
	void LoadMaterial(const string& directryPath, const string& filename);
	bool LoadTexture(const string& directoryPath, const string& filename);


	// setter
	//void SetModel(Model* model) { this->model = model; }
	void SetShaderNum(int shaderNum) { this->shaderNum = shaderNum; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetSize(XMFLOAT3 size) { this->size = size; }
	void SetRadius(float r) { this->r = r; }
	void SetBillboard(bool flg) { isBillboard = flg; }
	void SetBillboardY(bool flg) { isBillboardY = flg; }


	// getter
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetSize() { return size; }
	float GetRadius() { return r; }


#pragma endregion

private:

#pragma region �\����

	struct ConstBufferDataB0
	{
		XMMATRIX mat; // 3d�ϊ��s��
		XMFLOAT3 cameraPos;
		XMFLOAT3 color;
		//float colorR; // �F�iRGBA�j
		//float colorG;
		//float colorB;
		//float colorA;
	};

	// �萔�o�b�t�@�p�f�[�^�\����b1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv; //	uv���W
	};

	struct Material
	{
		string name;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float alpha;
		string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
#pragma endregion


#pragma region	directX

	// ���L����ϐ�
	static ComPtr<ID3D12RootSignature> rootSignature[2]; // ���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState[2]; // �p�C�v���C���X�e�[�g
	static XMMATRIX matProjection; // �ˉe�s��
	static const int SRVCount = 512; // �e�N�X�`���̍ő喇��
	static XMMATRIX matView;

	static ComPtr<ID3D12GraphicsCommandList> commandList;
	static ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;



	// 1�����Ƃɕς���ϐ�
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff; // �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;

	ComPtr<ID3D12Resource> texBuff;

	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW  ibView{}; // �C���f�b�N�X�o�b�t�@�r���[

#pragma endregion


// eye target�̓R���X�g���N�^����
	static Camera* camera;
	static const int vertexCount = 4;
	static const int indexCount = 3 * 2;

	// ���_�f�[�^�z��
	static Vertex vertices[vertexCount];
	static unsigned short indices[indexCount];


	XMMATRIX matWorld; // ���[���h�s��
	XMMATRIX matScale; // �X�P�[�����O�s��
	XMMATRIX matRot; // ��]�s��
	XMMATRIX matTrans; // ���s�ړ��s��

	XMFLOAT3 scale = { 1,1,1 };     // �X�P�[��


	UINT texNumber = 0; //	�e�N�X�`���ԍ�

	// �ǂ��炩�ɒl�����ăI�u�W�F�N�g�̑傫��������Ă���(��ɓ����蔻��p)
	XMFLOAT3 size = { 0,0,0 }; // ��`�̑傫��
	float r = 0; // ���a



	int shaderNum;
	Material material;

	// �r���{�[�h�p
	bool isBillboard = false;
	bool isBillboardY = false;


};
