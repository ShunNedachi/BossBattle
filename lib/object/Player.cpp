#include "Player.h"

// �ÓI�ϐ��̎���
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

}

void Player::Update()
{
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	const int XINPUT_MOVE_STICK = xinput->MoveStick(0, xinput_LS);

	// �ړ�����
	if (XINPUT_MOVE_STICK & XINPUT_STICK_UP ||
		XINPUT_MOVE_STICK & XINPUT_STICK_DOWN||
		XINPUT_MOVE_STICK & XINPUT_STICK_LEFT ||
		XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
	{
		//
		XMFLOAT3 eyeDir = Camera::GetEyeDir();

		XMVECTOR eyeDirV = { eyeDir.x,eyeDir.y,eyeDir.z,0 };
		XMFLOAT3 up = Camera::GetUp();
		XMVECTOR upV = { up.x,up.y,up.z,0 };

		// ����pos��ύX���āA�Ƃ肠�������������Ɉړ�
		if (XINPUT_MOVE_STICK & XINPUT_STICK_UP)
		{
			playerPos.x -= eyeDir.x;
			playerPos.z -= eyeDir.z;
		}
		else if (XINPUT_MOVE_STICK & XINPUT_STICK_DOWN)
		{
			playerPos.x += eyeDir.x;
			playerPos.z += eyeDir.z;
		}


		if (XINPUT_MOVE_STICK & XINPUT_STICK_LEFT)
		{
			// eyeDirV �Ɓ@up�̊O��
			XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upV, eyeDirV));
			XMFLOAT3 move;
			DirectX::XMStoreFloat3(&move, moveV);

			playerPos.x -= move.x;
			playerPos.z -= move.z;
		}
		else if (XINPUT_MOVE_STICK & XINPUT_STICK_RIGHT)
		{
			// eyeDirV �Ɓ@up�̊O��
			XMVECTOR moveV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(eyeDirV,upV));
			XMFLOAT3 move;
			DirectX::XMStoreFloat3(&move, moveV);

			playerPos.x -= move.x;
			playerPos.z -= move.z;
		}

	}


	// �����܂�

	//	�X�V
	objPlayer->SetPosition(playerPos);
	objPlayer->SetRotation(playerRot);
	objPlayer->SetScale(playerScale);

	objPlayer->Update();
}

void Player::Draw()
{
	objPlayer->Draw();
}
