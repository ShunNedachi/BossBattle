#pragma once
#include<DirectXMath.h>
#include"Object.h"
#include"Light.h"

struct AttackSize
{
	DirectX::XMFLOAT3 squareSize = {0,0,0};
	float ballRadius = 0;
};

class AttackBase
{
private:
	// �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;


public:
	// �֐�


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="startFrame">�U���O�̑ҋ@�t���[��</param>
	/// <param name="stopFrame">�U����̍d���t���[��</param>
	/// <param name="attackFrame">�U���p�̎����t���[��</param>
	/// <param name="size">�U���͈̔�</param>
	/// <param name="comboInputTime"></param>
	AttackBase(int startFrame,int attackFrame, int stopFrame,int comboInputTime = 0);
	~AttackBase();

	// update
	void Update();
	void Draw(Light& light);

	// �U���J�n�p
	void SetAttackStart();
	void SetObject(Object* object) { this->object = object; }

	// get
	XMFLOAT3 GetAttackPos() { return object->GetPosition(); }
	bool GetIsAttack() { return isAttack; }
	bool GetAttackStart() { return attackStart; }

	/// <summary>
	/// �U�����������Ă���̂�
	/// </summary>
	/// <param name="pos">���肵�������̂̈ʒu</param>
	/// <param name="size">��`�̃T�C�Y</param>
	/// <param name="r">���a</param>
	/// <returns></returns>
	bool Hit(XMFLOAT3 pos, XMFLOAT3 size = { 0,0,0 }, float r = 0);

private:

	// �U���J�n
	bool attackStart = false;

	// �U���O�̑ҋ@�t���[��
	int startFrame = 0;
	bool isStart = false;
	// �U���̎����t���[��
	int attackFrame = 0;

	// �U�������ǂ���
	bool isAttack = false;


	// �d�������ǂ���
	bool isEnd = false;
	// �U����̍d���t���[��
	int stopFrame = 0;
	
	// �A���U���p��t����
	float comboInputTime = 0;
	
	//// �U���̔���͈͗p
	//AttackSize size;

	// �o�߃t���[��(�v�Z�p)
	int elapsedFrame = 0;
	
	// �U���̌����ڗp
	Object* object = nullptr;

	//
};

