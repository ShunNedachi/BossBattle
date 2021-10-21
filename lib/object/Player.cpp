#include "Player.h"

// 静的変数の実体
Player* Player::instance = nullptr;

Player* Player::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Player();
	}

	return instance;
}

void Player::Destroy()
{
	if (objPlayer != nullptr)delete objPlayer;
	objPlayer = nullptr;

	if (attack != nullptr)delete attack;
	attack = nullptr;

	delete instance;
	instance = nullptr;
}

void Player::Init(const String& filename)
{
	objPlayer = new Object(NORMAL,filename);

	position = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };

	AttackSize size;
	size.squareSize = XMFLOAT3(3, 1, 1);

	attack = new AttackBase(0, 10, 100, size);
	attackObj = new Object(NORMAL,"sphere");
	attackObj->SetScale({ 10,10,2 });

	attack->SetObject(attackObj);

	objPlayer->SetScale(scale);

}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	const int XINPUT_MOVE_STICK = xinput->MoveStick(0, xinput_LS);
	const bool XINPUT_TRIGGER_A = xinput->TriggerButtom(0, xinput_A);

	// 移動処理

	// 入力確認
	if (XINPUT_MOVE_STICK & XINPUT_STICK_UP ||
		XINPUT_MOVE_STICK & XINPUT_STICK_DOWN||
		XINPUT_MOVE_STICK & XINPUT_STICK_LEFT ||
		XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
	{


		//	攻撃中じゃないときに移動可能
		if (!attack->GetAttackStart())
		{
			//
			XMFLOAT3 eyeDir = Camera::GetEyeDir();

			XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
			XMFLOAT3 up = Camera::GetUp();
			XMVECTOR upV = { up.x,up.y,up.z,0 };

			// 直接posを変更して、とりあえず視線方向に移動
			if (XINPUT_MOVE_STICK & XINPUT_STICK_UP)
			{
				position.x -= eyeDir.x;
				position.z -= eyeDir.z;
			}
			else if (XINPUT_MOVE_STICK & XINPUT_STICK_DOWN)
			{
				position.x += eyeDir.x;
				position.z += eyeDir.z;
			}


			if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT)
			{
				// eyeDirV と　upの外積
				XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));

				position.x -= moveV.m128_f32[0];
				position.z -= moveV.m128_f32[2];
			}
			else if (XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
			{
				// eyeDirV と　upの外積
				XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(eyeDirV, upV));


				position.x -= moveV.m128_f32[0];
				position.z -= moveV.m128_f32[2];
			}



			objPlayer->SetPosition(position);
			attackObj->SetPosition(position);

		}

	}
	// ここまで

	// 回転設定用 　yに+90しているのはモデルの初期値に対するものあとで変更
	// 攻撃中は回転を止める
	if(!attack->GetAttackStart())objPlayer->SetRotation({ 0, -1 *Camera::GetPhi() + 90,0 });




	// 攻撃用処理
	
	if (XINPUT_TRIGGER_A)attack->SetAttackStart();
	

	// ここまで



	//	更新

	objPlayer->Update();

	attackObj->Update();
	attack->Update();
}

void Player::Draw()
{
	objPlayer->Draw();

	if (attack->GetIsAttack())attackObj->Draw();
}
