#include "FrameFixed.h"
#include"Setting.h"
#include<time.h>

int FrameFixed::FPS = 60;
FrameFixed* FrameFixed::instance = nullptr;

FrameFixed* FrameFixed::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new FrameFixed();
	}

	return instance;
}

void FrameFixed::Destroy()
{
	delete instance;

	instance = nullptr;
}

void FrameFixed::PreWait()
{
	if (count == 0)
	{
		startTime = clock();
	}
	if (count == GAME_FRAME)
	{
		startTime = clock();
		count = 0;
	}
	count++;

}

void FrameFixed::PostWait()
{
	TookTime = clock() - startTime;
	WaitTime = count * 1000 / GAME_FRAME - TookTime;
	if (WaitTime > 0)
	{
		Sleep(WaitTime);
	}
}

