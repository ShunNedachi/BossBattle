#include "Input.h"
#include <DirectXMath.h>
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Microsoft::WRL::ComPtr<IDirectInput8> dinput;
LPVOID padDate;
LPDIRECTINPUTDEVICE8 devPad;

// デバイス発見時に実行される
BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	HRESULT result;
	result = dinput->CreateDevice(ipddi->guidInstance, &devPad, NULL);



	return DIENUM_CONTINUE;
}

bool Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// キーボードデバイスの生成	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// マウスデバイスの生成
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	result = devMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		assert(0);
		return result;
	}


	//　ゲームパッド用 接続ない時エラー
	// デバイスの列挙
	dinput->EnumDevices(DI8DEVTYPE_GAMEPAD, DeviceFindCallBack, padDate, DIEDFL_ATTACHEDONLY);

	if (devPad == nullptr)isPad = false;
	else isPad = true;

	if (isPad)
	{
		result = devPad->SetDataFormat(&c_dfDIJoystick);

		// 軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

		if (FAILED(devPad->SetProperty(DIPROP_AXISMODE, &diprop.diph))) return false;

		// X軸の値の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;
		if (FAILED(devPad->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		if (FAILED(devPad->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}

		diprg.diph.dwObj = DIJOFS_RX;
		if (FAILED(devPad->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}

		diprg.diph.dwObj = DIJOFS_RY;
		if (FAILED(devPad->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}



		// 強調モードの設定
		result = devPad->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		if (FAILED(result))
		{
			assert(0);
			return result;
		}


	}


	return true;
}

void Input::Update()
{
	HRESULT result;

	result = devkeyboard->Acquire();	// キーボード動作開始
	result = devMouse->Acquire(); // マウスの動作開始


	if (isPad)
	{
		// 接続ない時エラー
		result = devPad->Acquire(); // ゲームパッドの動作開始
		result = devPad->Poll(); // ポーリング開始


		// 前回のボタン入力を保存
		padStatePost = padState;

		// ゲームパッドの状態取得
		// ゲームパッド接続ない時エラー
		result = devPad->GetDeviceState(sizeof(DIJOYSTATE), &padState);



	}


	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	// 前回のマウス入力を保存
	pastMouseState = mouseState;

	// 前回のaボタン入力を保存
	postA_BottonDown = preA_BottonDown;

	// キーの入力
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	// マウスの状態取得
	result = devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (key[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::ClickLeft(MyWindow* winapp)
{
	if (mouseState.rgbButtons[0] & (0x80))
	{
		/*assert(0);*/
		/*Sleep(40);*/

		GetCursorPos(&p);
		// スクリーン座標をクライアント座標に変換する
		ScreenToClient(winapp->GetHWND(), &p);

		p.x -= 1920 / 2;
		p.y -= 1080 / 2;

		return true;
	}

	return false;
}

POINT Input::GetMousePos(MyWindow* winapp)
{
	/*assert(0);*/
	/*Sleep(40);*/
	POINT mouse;

	GetCursorPos(&mouse);
	// スクリーン座標をクライアント座標に変換する
	ScreenToClient(winapp->GetHWND(), &mouse);

	return mouse;

}




Input::~Input()
{
	if (isPad)
	{
		devPad->Unacquire();
		devPad->Release();
	}
}

void Input::LStick(bool &isLstickLeft, bool &isLstickRight)
{

	// 無反応範囲
	DWORD unresponsive_range = 200;
	if (padState.lX < 200 && padState.lX != 0)
	{
		isLstickLeft = true;
	}
	else
	{
		isLstickLeft = false;
	}

	if (padState.lX > -200 && padState.lX != 0)
	{
		isLstickRight = true;
	}
	else
	{
		isLstickRight = false;
	}
}

bool Input::AButtom()
{
	if (padState.rgbButtons[0] & 0x80)
	{
		return true;
	}
	else
	{
		return false;
	}


}

bool Input::TriggerAButtom()
{

	if (padState.rgbButtons[0] & 0x80)
	{
		preA_BottonDown = true;
	}
	else
	{
		preA_BottonDown = false;
	}


	if (preA_BottonDown && !postA_BottonDown)
	{

		return true;
	}
	else return false;



}


bool Input::PushButton(int ButtonNum)
{
	if (padState.rgbButtons[ButtonNum])
	{
		return true;
	}
	
	return false;
}

bool Input::TriggerButton(int ButtonNum)
{
	// 異常な引数を検出
	//assert(0 <= ButtonNum && ButtonNum <= 32);

	// 前回が0で、今回が0でなければトリガー
	if (!padStatePost.rgbButtons[ButtonNum] &&
		padState.rgbButtons[ButtonNum]) {
		return true;
	}

	// トリガーでない
	return false;
}

void Input::PadMove(POINT &pad)
{
	pad.x = padState.lRx;
	pad.y = padState.lRy;
}

bool Input::IsStickUse()
{
	// 無反応範囲
	DWORD unresponsive_range = 200;
	if (/*padState.lRx < 200 &&*/ padState.lRx != 0)
	{
		if (/*padState.lRy > -200 &&*/ padState.lRy != 0)
		{
			return false;
		}
	}
	else
	{
		return true;
	}

	return true;
}

// 十字キーを押したとき
bool Input::TriggerCrossKey(int keyNum)
{
	// トリガー入力
	if (padState.rgdwPOV[0] != 0xFFFFFFFF && padStatePost.rgdwPOV[0] == 0xFFFFFFFF)
	{

		float rad = DirectX::XMConvertToRadians(padState.rgdwPOV[0] / 100.0f);

		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			if(keyNum == CROSSKEY_LEFT)return true;
		}
		else if (x > 0.01f)
		{
			if (keyNum == CROSSKEY_RIGHT)return true;
		}
		
		if (y > 0.01f)
		{
			if (keyNum == CROSSKEY_UP)return true;
		}
		else if (y < -0.01f)
		{
			if (keyNum == CROSSKEY_DOWN)return true;
		}
	}
	
	return false;
}