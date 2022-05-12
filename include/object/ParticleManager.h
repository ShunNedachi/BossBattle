#pragma once
#include"Object.h"

class ParticleManager
{
private:

	// �G�C���A�X
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using string = std::string;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	ParticleManager();
	~ParticleManager();

#pragma region �����o�[�֐�

	// �����������@�S�̂ň��̂�init����
	static void Init(MyDirectX12* my12);
	static void CreatePiplineStateOBJ();


	void Draw();

	void Update();

	void CreateModel();
	void LoadMaterial(const string& directryPath, const string& filename);
	bool LoadTexture(const string& directoryPath, const string& filename);



	// setter
	//void SetModel(Model* model) { this->model = model; }

	void SetBillboard(bool flg) { isBillboard = flg; }
	void SetBillboardY(bool flg) { isBillboardY = flg; }


	static ComPtr<ID3D12Device> GetDevice() { return device; }

private:

	// �����܂Ƃߗp

	void SetDrawSetting();
	void DrawCommand();

	// �萔�o�b�t�@�̐����p
	template<typename T>
	HRESULT CreateConstBuffer(ComPtr<ID3D12Resource>& buffer);

	// �o�b�t�@�̍X�V�p
	template<typename T>
	void UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData);

#pragma endregion

private:

	#pragma region �\����

	struct ConstBufferDataB0
	{
		XMMATRIX mat; // 3d�ϊ��s��
		XMFLOAT3 cameraPos;
		float pad; // �؃f�B���O
		XMMATRIX billboardMat; // �r���{�[�h�s��
		XMFLOAT4 color;
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


	#pragma region	directX�p�ϐ�

	// ���L����ϐ�
	static ComPtr<ID3D12RootSignature> rootSignature; // ���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState; // �p�C�v���C���X�e�[�g
	//static const int SRVCount = 512; // �e�N�X�`���̍ő喇��

	static ComPtr<ID3D12GraphicsCommandList> commandList;
	static ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;

	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff;

	// �萔�o�b�t�@��
	static int constBuffNum;


	// 1�����Ƃɕς���ϐ�
	ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff; // �C���f�b�N�X�o�b�t�@


	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;


	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW  ibView{}; // �C���f�b�N�X�o�b�t�@�r���[

#pragma endregion

	XMMATRIX matScale; // �X�P�[�����O�s��
	XMMATRIX matRot; // ��]�s��
	XMMATRIX matTrans; // ���s�ړ��s��

	UINT texNumber = 0; //	�e�N�X�`���ԍ�

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;

	// �r���{�[�h�p
	bool isBillboard = false;
	bool isBillboardY = false;

};

template<typename T>
HRESULT ParticleManager::CreateConstBuffer(ComPtr<ID3D12Resource>& buffer)
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
void ParticleManager::UpdateBuffer(ComPtr<ID3D12Resource>& buffer, T& bufferData)
{
	T* map = nullptr;
	HRESULT result;

	result = buffer->Map(0, nullptr, (void**)&map);
	*map = *(&bufferData);
	buffer->Unmap(0, nullptr);
}