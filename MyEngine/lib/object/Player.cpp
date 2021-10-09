#include "Player.h"

// Ã“I•Ï”‚ÌŽÀ‘Ì
Player* Player::instance = nullptr;
Object* Player::objPlayer = nullptr;
DirectX::XMFLOAT3 Player::playerPos = { 0,0,0 };
DirectX::XMFLOAT3 Player::playerRot = { 0,0,0 };
DirectX::XMFLOAT3 Player::playerScale = { 1,1,1 };

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
	delete instance;
	instance = nullptr;


	delete objPlayer;
	objPlayer = nullptr;
	
}

void Player::Init(const std::string& filename)
{
	objPlayer = new Object(NORMAL);
	objPlayer->CreateModel(filename);
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	//if (input->PushKey(DIK_W))
	//{
	//	playerPos.z++;
	//}
	//else if (input->PushKey(DIK_S))
	//{
	//	playerPos.z--;
	//}
	//else if (input->PushKey(DIK_A))
	//{
	//	playerPos.x--;
	//}
	//else if (input->PushKey(DIK_D))
	//{
	//	playerPos.x++;
	//}

	objPlayer->Update(playerPos, playerScale, playerRot);
}

void Player::Draw()
{
	objPlayer->Draw();
}
