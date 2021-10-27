#pragma once
#include<Windows.h>

// �V���O���g���p�^�[��
class FrameFixed
{
private:
	// �R���X�g���N�^
	FrameFixed() = default;
	// �f�X�g���N�^
	~FrameFixed() = default;

	// �֎~����
	void operator=(const FrameFixed & obj) {}
	FrameFixed(const FrameFixed & obj) {}

public:

	static FrameFixed* GetInstance();
	void Destroy();

	void PreWait();
	void PostWait(); 

private:

	static FrameFixed* instance;
	static int FPS; // �ݒ肵��fps��

	int startTime;
	int count;
	int TookTime;
	int WaitTime;
};

