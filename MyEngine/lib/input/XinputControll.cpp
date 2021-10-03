#include"XinputControll.h"

Xinput::Xinput()
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

void Xinput::Update()
{	
	// トリガー用
	pastState[0] = state[0];
	pastState[1] = state[1];
	pastState[2] = state[2];
	pastState[3] = state[3];

	// 更新
	XInputGetState(0, &state[0]); 
	XInputGetState(1, &state[1]); 
	XInputGetState(2, &state[2]); 
	XInputGetState(3, &state[3]);

	// 接続中のコントローラーの判断用
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

bool Xinput::PushButtom(int controller, short buttom)
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

	// Aボタン入力時
	if (buttom == xinput_A)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			return true;
		}
	}
	// Bボタン入力時
	if (buttom == xinput_B)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			return true;
		}
	}
	// Xボタン入力時
	if (buttom == xinput_X)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			return true;
		}
	}
	// Yボタン入力時
	if (buttom == xinput_Y)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			return true;
		}
	}
	// Rボタン入力時
	if (buttom == xinput_R)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			return true;
		}
	}
	// Lボタン入力時
	if (buttom == xinput_L)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			return true;
		}
	}
	// R3ボタン入力時
	if (buttom == xinput_R3)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			return true;
		}
	}
	// L3ボタン入力時
	if (buttom == xinput_L3)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			return true;
		}
	}
	// Lトリガー入力時
	if (buttom == xinput_LT)
	{
		if (state[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			// 左トリガーが押された
			return true;
		}
	}
	// Rトリガー入力時
	if (buttom == xinput_RT)
	{
		if (state[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return true;
		}
	}

	// START入力時
	if (buttom == xinput_START)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_START)return true;
	}
	// BACK入力時
	if (buttom == xinput_BACK)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)return true;
	}


	// 上入力時
	if (buttom == xinput_UP)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)return true;
	}

	// 下入力時
	if (buttom == xinput_DOWN)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)return true;
	}

	// 左入力時
	if (buttom == xinput_LEFT)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) return true;
	}
	// 右入力時
	if (buttom == xinput_RIGHT)
	{
		if (state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)return true;
	}

	return false;
}

bool Xinput::TriggerButtom(int controller, short buttom)
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


	// Aボタン入力時
	if (buttom == xinput_A)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_A){}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			return true;
		}
	}
	// Bボタン入力時
	if (buttom == xinput_B)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_B) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_B)
		{
			return true;
		}
	}	
	// Xボタン入力時
	if (buttom == xinput_X)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_X) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			return true;
		}
	}
	// Yボタン入力時
	if (buttom == xinput_Y)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			return true;
		}
	}	
	// Rボタン入力時
	if (buttom == xinput_R)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			return true;
		}
	}
	// Lボタン入力時
	if (buttom == xinput_L)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			return true;
		}
	}
	// R3ボタン入力時
	if (buttom == xinput_R3)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
		{
			return true;
		}
	}	
	// L3ボタン入力時
	if (buttom == xinput_L3)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
		{
			return true;
		}
	}
	// Lトリガー入力時
	if (buttom == xinput_LT)
	{
		if(pastState[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD){}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return true;
		}
	}
	// Rトリガー入力時
	if(buttom == xinput_RT)
	{
		if (pastState[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD){}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return true;
		}
	}

	// START入力時
	if (buttom == xinput_START)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_START) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_START)
		{
			return true;
		}
	}

	// START入力時
	if (buttom == xinput_BACK)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		{
			return true;
		}
	}

	// 上入力時
	if (buttom == xinput_UP)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			return true;
		}
	}
	// 下入力時
	if (buttom == xinput_DOWN)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			return true;
		}
	}
	// 左入力時
	if (buttom == xinput_LEFT)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			return true;
		}
	}
	// 右入力時
	if (buttom == xinput_RIGHT)
	{
		if (pastState[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {}
		else forTrigger = true;

		if (forTrigger &&
			state[count].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			return true;
		}
	}

	return false;
}

int Xinput::MoveStick(int controller, short stick)
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
	if (count == -1)return 0;

	if (stick == xinput_LS)
	{
		int moveState = 0;

		// デッドゾーン
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
			moveState = leftInput;
		}
		else if (state[count].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState = rightInput;
		}

		if (state[count].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState += topInput;
		}
		else if (state[count].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			moveState += bottomInput;
		}

		return moveState;
	}
	else if (stick == xinput_RS)
	{
		int moveState = 0;

		// デッドゾーン
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
			moveState = leftInput;
		}
		else if (state[count].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState = rightInput;
		}

		if (state[count].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += topInput;
		}
		else if (state[count].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			moveState += bottomInput;
		}

		return moveState;

	}

	return 0;
}