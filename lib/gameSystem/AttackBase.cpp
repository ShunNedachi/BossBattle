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
	// 攻撃スタート
	if (attackStart)
	{	
		// 経過フレーム数増加
		elapsedFrame++;
		

		// 攻撃開始前の待機中
		if (isStart)
		{
			// 待機フレームを過ぎたら
			if (elapsedFrame >= startFrame)
			{
				elapsedFrame = 0;
				isStart = false;
				isAttack = true;
			}
		}
		
		// 攻撃中の処理
		if (isAttack)
		{
			
			
			// 経過フレームが過ぎたら
			if (elapsedFrame >= attackFrame)
			{
				// 計算用の経過フレームを初期化に
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
		
		// 必要なものの初期化
		isStart = true;
	}
}

bool AttackBase::Hit(XMFLOAT3 pos, XMFLOAT3 size, float r)
{

	if (isAttack)
	{
		// 攻撃中の時に判定でtrueが通るように

		// 判定が矩形の場合
		// 初期値が入っていた場合通らない
		if (size.x != 0 && size.y != 0 && size.z != 0)
		{

			// 矩形の処理(後程OBBを入れる)

		}
		else if (r != 0)// 判定が球の場合
		{
			 return Collision::Sphere2Sphere(pos, object->GetPosition(), r, object->GetRadius());
		}
		// ここまで

	}
	return false;
}

void AttackBase::Draw()
{
	object->Draw();
}
