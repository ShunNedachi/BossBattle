#pragma once
#include<vector>
#include"Object.h"
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Sprite2D.h"

// �V���O���g���p�^�[��
class ObjectManager
{
public:
	// �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	ObjectManager() {};
	~ObjectManager() {};

	// �R���X�g���N�^�@�f�X�g���N�^
	static ObjectManager* GetInstance();
	static void Destroy();
	// ������
	static void Initialize(MyDirectX12* my12, MyWindow* window, Camera* camera);

	void Update();
	void Draw();

	#pragma region 3D�I�u�W�F�N�g�p�֐�

	// �I�u�W�F�N�g�ǉ��R�}���h
	void AddOBJ(const std::string& filename, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = {1,1,1}, 
		XMFLOAT3 rotation = { 0,0,0 },int drawShader = NORMAL);

	#pragma endregion


	#pragma region �X�v���C�g�p�֐�

	// �摜�f�[�^���[�h
	void LoadTexture(const wchar_t* filename, int textureNum);

	// �X�v���C�g����
	void AddSprite(int textureNum, XMFLOAT2 position = { 0,0 }, float rotation = 0.0f, 
		XMFLOAT2 anchorPoint = {0.5f,0.5f}, XMFLOAT4 color = { 1,1,1,1 });


#pragma endregion



private:

	static ObjectManager* instance;

	// .obj�p�z��
	static std::vector<Object*> objArray;
	// sprite�p�z��
	static std::vector<Sprite2D*> spriteArray;


};

