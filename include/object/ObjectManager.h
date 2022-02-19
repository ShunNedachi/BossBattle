#pragma once
#include<vector>
#include"Object.h"
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Sprite2D.h"
#include"Player.h"
#include<string>
#include"Model.h"
#include"Boss.h"
#include"Light.h"

// �V���O���g���p�^�[��
class ObjectManager
{
private:
	// �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;


private:
	ObjectManager() = default;
	~ObjectManager() = default;

	void operator=(const ObjectManager& obj) {}
	ObjectManager(const ObjectManager& obj) {}

	// private�֐�
	void CheckArray();

public:


	static ObjectManager* GetInstance();
	static void Destroy();

	static void Initialize(MyDirectX12* my12);

	// �I�u�W�F�N�g�̎󂯓n���p
	// ��ł�����scene���Ɏ����čs����update���e���ł���悤�ɕύX����
	static std::vector<Object*>* GetObjectArray() { return &objArray; }
	static std::vector<Sprite2D*>* GetSpriteArray() { return &spriteArray; }
	static Player* GetPlayer() { return player; }
	static Boss* GetBoss() { return boss; }
	static Light* GetLight() { return light; }

	void Update();
	//void Draw();

	#pragma region Player�֌W�֐�

	static void AddPlayer(const std::string& filename);

#pragma endregion


	#pragma region Enemy�֌W�֐�

	void AddBoss();

#pragma endregion


	#pragma region 3D�I�u�W�F�N�g�p�֐�

	// �I�u�W�F�N�g�ǉ��R�}���h
	void AddOBJ(const std::string& filename, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 scale = {1,1,1}, 
		XMFLOAT3 rotation = { 0,0,0 },int drawShader = NORMAL);

	void DeleteOBJ(int index);

	#pragma endregion


	#pragma region �X�v���C�g�p�֐�

	// �摜�f�[�^���[�h
	//void LoadTexture(const wchar_t* filename, int textureNum);

	// �X�v���C�g����
	//void AddSprite(int textureNum, const std::string& filename = "white.png", XMFLOAT2 position = { 0,0 }, float rotation = 0.0f,
	//	XMFLOAT2 anchorPoint = { 0.5f,0.5f }, XMFLOAT4 color = { 1,1,1,1 });

	void AddSprite(int textureNum, XMFLOAT2 position = { 0,0 }, float rotation = 0.0f,
		XMFLOAT2 anchorPoint = { 0.5f,0.5f }, XMFLOAT3 color = { 1,1,1 });

	// �폜
	void DeleteSprite(int index);

#pragma endregion



private:

	static Xinput* xinput;

	static ObjectManager* instance;

	// .obj�p�z��
	static std::vector<Object*> objArray;


	// sprite�p�z��
	static std::vector<Sprite2D*> spriteArray;

	// ���[�h����Ă���t�@�C���f�[�^��ۑ�
	//static std::vector<std::string> loadedFileArray;

	static Player* player;

	// Enemy ���u��
	static Boss* boss;

	// ���C�g�@���������͌���Ή����邩��
	static Light* light;

	// �N���A�����B���m�F�p
	bool isClear = false;
};

