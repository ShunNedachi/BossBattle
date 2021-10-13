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
	if (objPlayer != nullptr)
	{
		delete objPlayer;
		objPlayer = nullptr;
	}

	delete instance;
	instance = nullptr;
}

void Player::Init(const std::string& filename)
{
	objPlayer = new Object(NORMAL);
	objPlayer->CreateModel(filename);

	playerPos = { 0,0,0 };
	playerRot = { 0,0,0 };
	playerScale = { 1,1,1 };
	jumpInit = false;
	canJump = true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// 移動処理
	if (xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_UP ||
		xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_DOWN||
		xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_LEFT ||
		xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_RIGHT)
	{
		//
		XMFLOAT3 eyeDir = Camera::GetEyeDir();

		XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
		XMFLOAT3 up = Camera::GetUp();
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		// 直接posを変更して、とりあえず視線方向に移動
		if (xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_UP)
		{
			playerPos.x -= eyeDir.x;
			//playerPos.y += eyeDir.y;
			playerPos.z -= eyeDir.z;
			//playerVelo.z -= 0.1f;
		}
		else if (xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_DOWN)
		{
			playerPos.x += eyeDir.x;
			//playerPos.y -= eyeDir.y;
			playerPos.z += eyeDir.z;
			//playerVelo.z += 0.1f;
		}


		if (xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_LEFT)
		{
			// eyeDirV と　upの外積
			XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
			XMFLOAT3 move;
			DirectX::XMStoreFloat3(&move, moveV);

			playerPos.x -= move.x;
			playerPos.z -= move.z;
			//playerVelo.x -= 0.1f;
		}
		else if (xinput->MoveStick(0, xinput_LS) & XINPUT_STICK_RIGHT)
		{
			// eyeDirV と　upの外積
			XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(eyeDirV,upV));
			XMFLOAT3 move;
			DirectX::XMStoreFloat3(&move, moveV);

			playerPos.x -= move.x;
			playerPos.z -= move.z;
			//playerVelo.x += 0.1f;
		}

	}
	else
	{
		playerVelo.x = 0;
		playerVelo.z = 0;
	}

	// ここまで


	// ジャンプ処理

	if (xinput->TriggerButtom(0, xinput_A)&& canJump)
	{
		jumpInit = true;
	}

	// ジャンプ処理中のとき
	if (jumpInit)
	{
		playerVelo.y = 5;
		jumpInit = false;
		canJump = false;
	}

	// 落下処理 (プレイヤーが浮いているとき(仮)、後で地面に当たっていないときに変更)
	if(playerPos.y > 0)playerVelo.y -= 0.1f;
	else if (playerPos.y < 0) 
	{
		canJump = true;
		playerVelo.y = 0;
		playerPos.y = 0;
	}

	// ここまで


	// 加速最大値
	if (playerVelo.x > 1)playerVelo.x = 1;
	else if (playerVelo.x < -1)playerVelo.x = -1;
	if (playerVelo.z > 1)playerVelo.z = 1;
	else if (playerVelo.z < -1)playerVelo.z = -1;
	// ここまで

	playerPos.x += playerVelo.x;
	playerPos.y += playerVelo.y;
	playerPos.z += playerVelo.z;
	objPlayer->Update(playerPos, playerScale, playerRot);
}

void Player::Draw()
{
	objPlayer->Draw();
}
