#pragma once
#include<Windows.h>
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")

// コントローラーのボタン
#define xinput_X 0
#define xinput_Y 1
#define xinput_A 2
#define xinput_B 3
#define xinput_R 4
#define xinput_L 5
#define xinput_R3 6
#define xinput_L3 7
#define xinput_LT 8
#define xinput_RT 9
#define xinput_LS 10
#define xinput_RS 11
#define xinput_START 12
#define xinput_BACK 13


// コントローラーの十字キー
#define xinput_UP 14
#define xinput_DOWN 15
#define xinput_LEFT 16
#define xinput_RIGHT 17

#define leftInput 0x00000001
#define rightInput 0x00000002
#define topInput 0x00000010
#define bottomInput 0x00000020

// 接続した、コントローラーの番号を識別
struct SetController
{
	bool controller1 = false;
	bool controller2 = false;
	bool controller3 = false;
	bool controller4 = false;
};

class Xinput
{

public:

	Xinput();
	~Xinput() {};

	// 更新用
	void Update();

	// ボタンが押されたかどうか
	bool PushButtom(int controller, short buttom);
	bool TriggerButtom(int controller, short buttom);

	// スティックを動かしたかを判定する
	int MoveStick(int controller,short stick);


	// 接続中のコントローラー数を返す
	int IsControllNUM() { return controllerNum; }

	// 

private:

	XINPUT_STATE state[4] = {};
	// トリガー用
	XINPUT_STATE pastState[4] = {};


	int controllerNum = 4; // 接続されているコントローラーの数
	SetController pad ={};

};

