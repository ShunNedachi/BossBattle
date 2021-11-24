#include"XinputControll.h"
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

// �ÓI�����o�ϐ��̎��̐錾
XINPUT_STATE Xinput::state[4] = {};
XINPUT_STATE Xinput::pastState[4] = {};

int Xinput::controllerNum = 4;
SetController Xinput::pad = {};

Xinput* Xinput::instance;

void Xinput::Initialize()
{
	ZeroMemory(&state[0], sizeof(XINPUT_STATE));
	ZeroMemory(&state[1], sizeof(XINPUT_STATE));
	ZeroMemory(&state[2], sizeof(XINPUT_STATE));
	ZeroMemory(&state[3], sizeof(XINPUT_STATE));

	ZeroMemory(&pastState[0], sizeof(XINPUT_STATE));
	ZeroMemory(&pastState[1], sizeof(XINPUT_STATE));
	ZeroMemory(&pastState[2], sizeof(XINPUT_STATE));
	ZeroMemory(&pastState[3], sizeof(XINPUT_STATE));

}

Xinput* Xinput::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Xinput();
	}
	
	return instance;
}

void Xinput::Destroy()
{
	delete instance;

	instance = nullptr;
}

void Xinput::Update()
{	
	// �g���K�[�p
	pastState[0] = state[0];
	pastState[1] = state[1];
	pastState[2] = state[2];
	pastState[3] = state[3];

	// �X�V
	XInputGetState(0, &state[0]); 
	XInputGetState(1, &state[1]); 
	XInputGetState(2, &state[2]); 
	XInputGetState(3, &state[3]);

	// �ڑ����̃R���g���[���[�̔��f�p
	controllerNum = 0;
	if (state[0].dwPacketNumber != 0)
	{
		controllerNum++;
		pad.controller1 = true;
	}
	else pad.controller1 = false;

	if (state[1].dwPacketNumber != 0)
	{
		controllerNum++;
		pad.controller2 = true;
	}
	else pad.controller2 = false;

	if (state[2].dwPacketNumber != 0)
	{
		controllerNum++;
		pad.controller3 = true;
	}
	else pad.controller3 = false;

	if (state[3].dwPacketNumber != 0) 
	{
		controllerNum++; 
		pad.controller4 = true;
	}
	else pad.controller4 = false;
}

buttonState Xinput::PushButton(int controller)
{
	int count = -1;
	
	if (pad.controller1 && controller == 0)
	{
		count = 0;
	}
	else if (pad.controller2 && controller == 1 || !pad.controller1 && pad.controller2 && controller == 0)
	{
		count = 1;
	}
	else if (pad.controller3 && controller == 2 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && controller == 0
		|| !pad.controller1 && pad.controller2 && pad.controller3 && controller == 1)
	{
		count = 2;
	}
	else if (pad.controller4 && controller == 3 ||
		!pad.controller1 && !pad.controller2 && !pad.controller3 && pad.controller4 && controller == 0 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && pad.controller4 && controller == 1 ||
		!pad.controller1 && pad.controller2 && pad.controller3 && pad.controller4 && controller == 2)
	{
		count = 3;
	}

	if (count == -1)return false;

	buttonState result = 0;

	// A�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		result += XINPUT_BUTTON_A;
	}
	
	// B�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		result += XINPUT_BUTTON_B;
	}
	
	// X�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		result += XINPUT_BUTTON_X;
	}

	// Y�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		result += XINPUT_BUTTON_Y;
	}

	// R�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		result += XINPUT_BUTTON_R;
	}
	
	// L�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		result += XINPUT_BUTTON_L;
	}
	// R3�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		result += XINPUT_BUTTON_R3;
	}
	
	// L3�{�^�����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		result += XINPUT_BUTTON_L3;
	}
	
	// L�g���K�[���͎�
	if (state[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		// ���g���K�[�������ꂽ
		result += XINPUT_BUTTON_LT;
	}
	
	// R�g���K�[���͎�
	if (state[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		result += XINPUT_BUTTON_RT;
	}
	

	// START���͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_START)result += XINPUT_BUTTON_START;
	
	// BACK���͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)result += XINPUT_BUTTON_BACK;
	


	// ����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)result += XINPUT_BUTTON_UP;

	// �����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)result += XINPUT_BUTTON_DOWN;
	

	// �����͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)result += XINPUT_BUTTON_LEFT;
	
	// �E���͎�
	if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)result += XINPUT_BUTTON_RIGHT;
	

	return result;
}

buttonState Xinput::TriggerButton(int controller)
{
	bool forTrigger = false;
	int count = -1;

	if (pad.controller1 && controller == 0)
	{
		count = 0;
	}
	else if (pad.controller2 && controller == 1 || !pad.controller1 && pad.controller2 && controller == 0)
	{
		count = 1;
	}
	else if (pad.controller3 && controller == 2 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && controller == 0
		|| !pad.controller1 && pad.controller2 && pad.controller3 && controller == 1)
	{
		count = 2;
	}
	else if (pad.controller4 && controller == 3 ||
		!pad.controller1 && !pad.controller2 && !pad.controller3 && pad.controller4 && controller == 0 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && pad.controller4 && controller == 1 ||
		!pad.controller1 && pad.controller2 && pad.controller3 && pad.controller4 && controller == 2)
	{
		count = 3;
	}
	if (count == -1)return false;

	buttonState  result = 0;

	// A�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_A) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		result += XINPUT_BUTTON_A;
	}
	// B�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_B) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		result += XINPUT_BUTTON_B;
	}
	

	// X�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_X) { forTrigger = false; }
		else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		result += XINPUT_BUTTON_X;
	}

	// Y�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		result += XINPUT_BUTTON_Y;
	}
	
	// R�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		result += XINPUT_BUTTON_R;
	}
	
	// L�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		result += XINPUT_BUTTON_L;
	}
	
	// R3�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		result += XINPUT_BUTTON_R3;
	}
		
	// L3�{�^�����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		result += XINPUT_BUTTON_L3;
	}
	
	// L�g���K�[���͎�
	if(pastState[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD){ forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		result += XINPUT_BUTTON_LT;
	}
	
	// R�g���K�[���͎�
	if (pastState[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD){ forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		result += XINPUT_BUTTON_RT;
	}
	

	// START���͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_START) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		result += XINPUT_BUTTON_START;
	}

	// BACK���͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		result += XINPUT_BUTTON_BACK;
	}
	

	// ����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		result += XINPUT_BUTTON_UP;
	}

	// �����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		result += XINPUT_BUTTON_DOWN;
	}
	
	// �����͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		result += XINPUT_BUTTON_LEFT;
	}
	
	// �E���͎�
	if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) { forTrigger = false; }
	else forTrigger = true;

	if (forTrigger &&
		state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		result += XINPUT_BUTTON_RIGHT;
	}
	

	return result;
}

stickState Xinput::MoveStick(int controller, short stick)
{
	int count = -1;

	// �ǂ̃R���g���[���[�����ʂ���̂�
	if (pad.controller1 && controller == 0)
	{
		count = 0;
	}
	else if (pad.controller2 && controller == 1 || !pad.controller1 && pad.controller2 && controller == 0)
	{
		count = 1;
	}
	else if (pad.controller3 && controller == 2 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && controller == 0
		|| !pad.controller1 && pad.controller2 && pad.controller3 && controller == 1)
	{
		count = 2;
	}
	else if (pad.controller4 && controller == 3 ||
		!pad.controller1 && !pad.controller2 && !pad.controller3 && pad.controller4 && controller == 0 ||
		!pad.controller1 && !pad.controller2 && pad.controller3 && pad.controller4 && controller == 1 ||
		!pad.controller1 && pad.controller2 && pad.controller3 && pad.controller4 && controller == 2)
	{
		count = 3;
	}
	if (count == -1)return 0;

	if (stick == XINPUT_BUTTON_LS)
	{
		char moveState = 0;

		// �f�b�h�]�[��
		if ((state[count].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state[count].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state[count].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state[count].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state[count].Gamepad.sThumbLX = 0;
			state[count].Gamepad.sThumbLY = 0;
		}

		if (state[count].Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState = XINPUT_STICK_LEFT;
		}
		else if (state[count].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState = XINPUT_STICK_RIGHT;
		}

		if (state[count].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_DOWN;
		}
		else if (state[count].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_UP;
		}

		return moveState;
	}
	else if (stick == XINPUT_BUTTON_RS)
	{
		char moveState = 0;

		// �f�b�h�]�[��
		if ((state[count].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state[count].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(state[count].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state[count].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			state[count].Gamepad.sThumbRX = 0;
			state[count].Gamepad.sThumbRY = 0;
		}

		if (state[count].Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_LEFT;
		}
		else if (state[count].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_RIGHT;
		}

		if (state[count].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_DOWN;
		}
		else if (state[count].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += XINPUT_STICK_UP;
		}

		return moveState;

	}

	return 0;
}