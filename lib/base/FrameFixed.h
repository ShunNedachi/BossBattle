#pragma once
#include<Windows.h>

// シングルトンパターン
class FrameFixed
{
private:
	// コンストラクタ
	FrameFixed() = default;
	// デストラクタ
	~FrameFixed() = default;

	// 禁止項目
	void operator=(const FrameFixed & obj) {}
	FrameFixed(const FrameFixed & obj) {}

public:

	static FrameFixed* GetInstance();
	void Destroy();

	void PreWait();
	void PostWait(); 

private:

	static FrameFixed* instance;
	static int FPS; // 設定したfps数

	int startTime;
	int count;
	int TookTime;
	int WaitTime;
};

