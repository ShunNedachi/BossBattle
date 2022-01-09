#pragma once

#include <Windows.h>
#include <wrl.h>
#include"MyWindow.h"

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

#define CROSSKEY_UP 0
#define CROSSKEY_DOWN 1
#define CROSSKEY_LEFT 2
#define CROSSKEY_RIGHT 3


// シングルトンパターン

/// <summary>
/// 入力
/// </summary>
class Input
{
private: // エイリアス
		 // Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: 
	static Input* GetInstance();
	static void Destroy();

	// メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(HINSTANCE hInstance, HWND hwnd);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	bool ClickLeft(MyWindow* winapp);

	POINT GetPoint() { return p; }

	POINT GetMousePos(MyWindow* winapp);

	bool GetIsPad() { return isPad; }

	// ゲームパッド用関数

	void LStick(bool &isLstickLeft, bool &isLstickRight);


	bool AButtom();
	bool TriggerAButtom();

	bool PushButton(int ButtonNum);
	bool TriggerButton(int ButtonNum);
	bool IsStickUse();
	

	void PadMove(POINT &pad);
	
	bool TriggerCrossKey(int KeyNum);


private:

	Input() = default;
	~Input() = default;
	void operator=(const Input & obj) {}
	Input(const Input & obj) {}

	// インスタンス
	static Input* instance;
	static bool isPad;

	// メンバ変数
	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> devMouse;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	DIMOUSESTATE mouseState;
	DIMOUSESTATE pastMouseState;

	POINT p;

	DIJOYSTATE padState;


	bool pushAButtom = false;

	POINT mousePos;

	bool preA_BottonDown = false;
	bool postA_BottonDown = false;


	// ボタン用配列
	DIJOYSTATE padStatePost; // 前フレームのボタン入力用
};