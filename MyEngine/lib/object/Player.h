#pragma once
#include"Object.h"
#include"Sprite2D.h"
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
	void Init(const std::string& filename);
	void Update();
	void Draw();

	// setter
	void SetPos(XMFLOAT3 pos) { playerPos = pos; }
	void SetRot(XMFLOAT3 rot) { playerRot = rot; }
	void SetScale(XMFLOAT3 scale) { playerScale = scale; }

	// getter
	XMFLOAT3 GetPos() { return playerPos; }
	XMFLOAT3 GetRot() { return playerRot; }
	XMFLOAT3 GetScale() { return playerScale; }


protected:
	// �R���X�g���N�^
	Player() {};
	// �f�X�g���N�^
	~Player() {};


private:
	// �ϐ�

	static Player* instance;

	Object* objPlayer = nullptr;
	XMFLOAT3 playerPos = {0,0,0};
	XMFLOAT3 playerRot = {0,0,0};
	XMFLOAT3 playerScale = {1,1,1};


	XMFLOAT3 playerVelo = { 0,0,0 };

	bool canJump = true;
	bool jumpInit = false;
};

