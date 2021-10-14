#pragma once
#include<string>
#include<wrl.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Model
{
private:
	// �G�C���A�X
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using String = std::string;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:
	
	// �\����
	struct Vertex
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv; //	uv���W
	};

	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;

		float alpha;
		std::string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;

			textureFilename = "white.png";
		}
	};

	// �}�e���A���萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};


public:
	 // �����o�֐�

	Model() {};
	~Model();

	// ������
	static void Initialize(ID3D12Device* device);

	void CreateModel(const String& modelname,int textureNum);

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	void CreateBuffer();

	// getter

	ComPtr<ID3D12Resource> GetVertBuff() { return vertBuff; }
	ComPtr<ID3D12Resource> GetIndexBuff() { return indexBuff; }
	ComPtr<ID3D12DescriptorHeap> GetDescHeap() {return descHeap; }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView; }
	D3D12_INDEX_BUFFER_VIEW GetIbView() { return ibView; }
	ComPtr<ID3D12Resource> GetConstBuff() { return constBuff; }
	ComPtr<ID3D12Resource> GetTexBuff() { return texBuff; }

	std::vector<Vertex> GetVertices() { return vertices; }
	std::vector<unsigned short> GetIndices() { return indices; }
	Material GetMaterial() { return material; }

	int GetTexNum() { return texNum; }

private:

	// �ÓI�����o�ϐ�
	
	static ID3D12Device* device;
	//static UINT descriptorHandleIncrementSize;

	// �����o�ϐ�

	ComPtr<ID3D12DescriptorHeap> descHeap;

	// ���_�o�b�t�@�A�o�b�t�@�r���[
	ComPtr<ID3D12Resource> vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// �C���f�b�N�X�o�b�t�@�A�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff;
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// �萔�o�b�t�@�@�}�e���A���p
	ComPtr<ID3D12Resource> constBuff;

	// ���_�f�[�^�z��
	std::vector<Vertex> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	
	// �}�e���A��
	Material material;
	ComPtr<ID3D12Resource> texBuff;

	// �e�N�X�`���ԍ�
	UINT texNum = 0;

};

