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

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="pos">�U���҂̈ʒu</param>
	/// <param name="damage"></param>
	void RecieveDamage(XMFLOAT3 pos, float damage);

	// setter ��ŏC��
	void SetPos(XMFLOAT3 pos) { position = pos; }
	void SetRot(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scale) { scale = scale; }
	void SetSize(XMFLOAT3 size) { objPlayer->SetSize(size); }
	void SetRadius(float r) { objPlayer->SetRadius(r); }

	// getter
	XMFLOAT3 GetPos() { return position; }
	XMFLOAT3 GetRot() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetSize() { return objPlayer->GetSize(); }
	XMFLOAT3 GetAttackPos() { return attack->GetAttackPos(); }
	float GetRadius() { return objPlayer->GetRadius(); }
	bool GetIsDamage() { return isDamage; }
	int GetDamage() { return damage; }

	// �U������p
	AttackBase* GetAttack() { return attack; }
	Object* GetOBJ() { return objPlayer; }


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
	// helth�l
	float health = 10;
	// �o���l��
	float experience;
	// �_���[�W��������Ă���̂��̔���
	bool isDamage = false;
	// �_���[�W��Ԃ̌o�߃t���[����
	int damageCount;
	// �_���[�W��Ԃ̍ő�t���[����
	const int DAMAGE_FRAME = 20;

	int damage = 1;

	// �̗͗p
	Sprite2D* healthBarSprite = nullptr;
	// �̗̓o�[�p
	std::vector<Sprite2D*> healthSprite;


	// �U���p
	AttackBase* attack;
	Object* attackObj;

	// �v���C���[�̐��ʃx�N�g��
	XMVECTOR move;

};

