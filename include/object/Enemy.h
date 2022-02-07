#pragma once
#include"Object.h"
#include"Setting.h"

// ���N���X
class Enemy
{

protected:
	// �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	// �֐�
	Enemy() {}
	virtual ~Enemy() { Destroy(); }

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void Destroy();

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="pos">�U���҂̈ʒu</param>
	/// <param name="damage">�󂯂��_���[�W</param>
	/// <param name="knockback">�m�b�N�o�b�N����̂�</param>
	void RecieveDamage(XMFLOAT3 pos, float damage,bool knockback = true);


	// setter
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetSize(XMFLOAT3 size) { obj->SetSize(size); }
	void SetRadius(float r) { obj->SetRadius(r); }

	// getter
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetRotation() { return rotation;}
	XMFLOAT3 GetSize() { return obj->GetSize(); }
	float GetRadius() { return obj->GetRadius(); }

	int GetDamage() { return damage; }
	bool GetIsDamage() { return isDamage; }

	float GetHealth() { return health; }
	float GetEXP() { return experience; }

	// �U������p
	//AttackBase* GetAttack() { return attack; }
	Object* GetOBJ() { return obj; }
	std::vector<Object*> GetAttackObj() { return attackObjs; }
	std::vector<Object*>* GetAttackObjPointer() { return &attackObjs; }

protected:
	Object* obj = nullptr;

	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 scale = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };

	// �X�e�[�^�X�p
	float health = 3;
	float experience = 10;

	// �_���[�W��������Ă���̂��̔���
	bool isDamage = false;
	// �_���[�W��Ԃ̌o�߃t���[����
	int damageCount;
	// �_���[�W��Ԃ̍ő�t���[����
	const int DAMAGE_FRAME = 10;

	// ���ݒ�̃_���[�W��
	int damage = 1;

	std::vector<Object*> attackObjs;

	XMFLOAT3 knockbackPos;
	bool isKnockBack = false;
	Easing knockBackXEasing;
	Easing knockBackZEasing;
};

