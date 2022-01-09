#pragma once
#include<Windows.h>
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")


#pragma region Define

	// コントローラーのボタン
	// 0x00000000
	#define XINPUT_BUTTON_X 0x0001
	#define XINPUT_BUTTON_Y 0x0002
	#define XINPUT_BUTTON_A 0x0004
	#define XINPUT_BUTTON_B 0x0008
	#define XINPUT_BUTTON_R 0x0010
	#define XINPUT_BUTTON_L 0x0020
	#define XINPUT_BUTTON_R3 0x0040
	#define XINPUT_BUTTON_L3 0x0080
	#define XINPUT_BUTTON_LT 0x0100
	#define XINPUT_BUTTON_RT 0x0200
	#define XINPUT_BUTTON_START 0x0400
	#define XINPUT_BUTTON_BACK 0x0800
	// コントローラーの十字キー
	#define XINPUT_BUTTON_UP 0x1000
	#define XINPUT_BUTTON_DOWN 0x2000
	#define XINPUT_BUTTON_LEFT 0x4000
	#define XINPUT_BUTTON_RIGHT 0x8000

	// 使用スティック識別用
	#define XINPUT_BUTTON_LS 0x00000400
	#define XINPUT_BUTTON_RS 0x00000800
	#define XINPUT_STICK_LEFT 0x01
	#define XINPUT_STICK_RIGHT 0x02
	#define XINPUT_STICK_UP 0x04
	#define XINPUT_STICK_DOWN 0x08

	typedef unsigned char stickState;
	typedef unsigned short buttonState;


	// 接続した、コントローラーの番号を識別
	struct SetController
	{
		bool controller1 = false;
		bool controller2 = false;
		bool controller3 = false;
		bool controller4 = false;
	};
	#pragma endregion


class Xinput
{
private:
	Xinput() = default;
	~Xinput() = default;

	void operator=(const Xinput& obj) {}
	Xinput(const Xinput& obj) {}


public:

	// デストラクタ
	static void Destroy();

	// 初期化
	static void Initialize();

	// 更新用
	void Update();

	// ボタンが押されたかどうか
	buttonState PushButton(int controller);
	buttonState TriggerButton(int controller);

	// スティックを動かしたかを判定する
	stickState MoveStick(int controller,short stick);


	// 接続中のコントローラー数を返す
	int IsControllNUM() { return controllerNum; }

	// 
	static Xinput* GetInstance();

private:

	


	// 変数

	static Xinput* instance;

	static XINPUT_STATE state[4];
	// トリガー用
	static XINPUT_STATE pastState[4];


	static int controllerNum; // 接続されているコントローラーの数
	static SetController pad;

};

