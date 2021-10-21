#pragma once
#include"Object.h"
#include"Sprite2D.h"
#include"AttackBase.h"
#include<string>

class Player
{
	// �V���O���g���p�^�[��

private:
	// �G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;
	using String = std::string;

public:
	// �֐�

	static Player* GetInstance();
	void Destroy();

	//
	void Init(const String& filename);
	void Update();
	void Draw();

	// setter
	void SetPos(XMFLOAT3 pos) { position = pos; }
	void SetRot(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scale) { scale = scale; }

	// getter
	XMFLOAT3 GetPos() { return position; }
	XMFLOAT3 GetRot() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }


protected:

private:
	// �R���X�g���N�^
	Player() = default;
	// �f�X�g���N�^
	~Player() = default;

	// �֎~����
	void operator=(const Player& obj) {}
	Player(const Player& obj){}

private:
	// �ϐ�

	static Player* instance;

	Object* objPlayer = nullptr;
	XMFLOAT3 position = {0,0,0};
	XMFLOAT3 rotation = {0,0,0};
	XMFLOAT3 scale = {1,1,1};

	// system�p
	float health;

	// �U���p
	AttackBase* attack;
	Object* attackObj;

	// ��]�m�F�p
	float rotX = 0; // x���p
	float rotY = 0; // y���p

};

