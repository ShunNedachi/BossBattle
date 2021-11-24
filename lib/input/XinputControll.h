#pragma once
#include<Windows.h>
#include<Xinput.h>

#pragma comment(lib,"xinput.lib")


#pragma region Define

	// �R���g���[���[�̃{�^��
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
	// �R���g���[���[�̏\���L�[
	#define XINPUT_BUTTON_UP 0x1000
	#define XINPUT_BUTTON_DOWN 0x2000
	#define XINPUT_BUTTON_LEFT 0x4000
	#define XINPUT_BUTTON_RIGHT 0x8000

	// �g�p�X�e�B�b�N���ʗp
	#define XINPUT_BUTTON_LS 0x00000400
	#define XINPUT_BUTTON_RS 0x00000800
	#define XINPUT_STICK_LEFT 0x01
	#define XINPUT_STICK_RIGHT 0x02
	#define XINPUT_STICK_UP 0x04
	#define XINPUT_STICK_DOWN 0x08

	typedef unsigned char stickState;
	typedef unsigned short buttonState;


	// �ڑ������A�R���g���[���[�̔ԍ�������
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

	// �f�X�g���N�^
	static void Destroy();

	// ������
	static void Initialize();

	// �X�V�p
	void Update();

	// �{�^���������ꂽ���ǂ���
	buttonState PushButton(int controller);
	buttonState TriggerButton(int controller);

	// �X�e�B�b�N�𓮂��������𔻒肷��
	stickState MoveStick(int controller,short stick);


	// �ڑ����̃R���g���[���[����Ԃ�
	int IsControllNUM() { return controllerNum; }

	// 
	static Xinput* GetInstance();

private:

	


	// �ϐ�

	static Xinput* instance;

	static XINPUT_STATE state[4];
	// �g���K�[�p
	static XINPUT_STATE pastState[4];


	static int controllerNum; // �ڑ�����Ă���R���g���[���[�̐�
	static SetController pad;

};

