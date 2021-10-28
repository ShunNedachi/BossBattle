#include "AttackBase.h"
#include"Collision.h"

AttackBase::AttackBase(int startFrame, int attackFrame, int stopFrame, int comboInputTime)
						:startFrame(startFrame),stopFrame(stopFrame),attackFrame(attackFrame),comboInputTime(comboInputTime)
{

}

AttackBase::~AttackBase()
{

}

void AttackBase::Update()
{
	// �U���X�^�[�g
	if (attackStart)
	{	
		// �o�߃t���[��������
		elapsedFrame++;
		

		// �U���J�n�O�̑ҋ@��
		if (isStart)
		{
			// �ҋ@�t���[�����߂�����
			if (elapsedFrame >= startFrame)
			{
				elapsedFrame = 0;
				isStart = false;
				isAttack = true;
			}
		}
		
		// �U�����̏���
		if (isAttack)
		{
			
			
			// �o�߃t���[�����߂�����
			if (elapsedFrame >= attackFrame)
			{
				// �v�Z�p�̌o�߃t���[������������
				elapsedFrame = 0;
				isAttack = false;
				isEnd = true;
			}
		}
		else if (isEnd)
		{
			if (elapsedFrame >= stopFrame)
			{
				elapsedFrame = 0;
				isEnd = false;
				attackStart = false;
			}
		}

		object->Update();
	}
}


void AttackBase::SetAttackStart()
{
	if (!attackStart)
	{
		attackStart = true;
		
		// �K�v�Ȃ��̂̏�����
		isStart = true;
	}
}

bool AttackBase::Hit(XMFLOAT3 pos, XMFLOAT3 size, float r)
{

	if (isAttack)
	{
		// �U�����̎��ɔ����true���ʂ�悤��

		// ���肪��`�̏ꍇ
		// �����l�������Ă����ꍇ�ʂ�Ȃ�
		if (size.x != 0 && size.y != 0 && size.z != 0)
		{

			// ��`�̏���(���OBB������)

		}
		else if (r != 0)// ���肪���̏ꍇ
		{
			 return Collision::Sphere2Sphere(pos, object->GetPosition(), r, object->GetRadius());
		}
		// �����܂�

	}
	return false;
}

void AttackBase::Draw()
{
	object->Draw();
}
