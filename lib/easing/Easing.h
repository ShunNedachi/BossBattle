#pragma once
#include<time.h>
#include<DirectXMath.h>
#include<math.h>

#define PI 3.14159265359

#define easeInFLAG 0x00000001
#define easeOutFLAG 0x00000002
#define easeInOutFLAG 0x00000004

#define easeInQuadFLAG 0x00000008
#define easeOutQuadFLAG 0x00000010
#define easeInOutQuadFLAG 0x00000020

#define easeInCubicFLAG 0x00000040
#define easeOutCubicFLAG 0x00000080
#define easeInOutCubicFLAG 0x00000100

#define easeInQuartFLAG 0x00000200
#define easeOutQuartFLAG 0x00000400
#define easeInOutQuartFLAG 0x00000800

#define easeInQuintFLAG 0x00001000
#define easeOutQuintFLAG 0x00002000
#define easeInOutQuintFLAG 0x00004000

#define easeInExpoFLAG 0x00008000
#define easeOutExpoFLAG 0x00010000
#define easeInOutExpoFLAG 0x00020000

#define easeInCircFLAG 0x00040000
#define easeOutCircFLAG 0x00080000
#define easeInOutCircFLAG 0x00100000

#define easeInBackFLAG 0x00200000
#define easeOutBackFLAG 0x00400000
#define easeInOutBackFLAG 0x00800000

#define easeInElasticFLAG 0x01000000
#define easeOutElasticFLAG 0x02000000
#define easeInOutElasticFLAG 0x04000000

#define easeInBounceFLAG 0x08000000
#define easeOutBounceFLAG 0x10000000
#define easeInOutBounceFLAG 0x20000000


// easing関数まとめクラス
class Easing
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

	// 関数

	/// <summary>
	/// イージングの情報設定用関数
	/// </summary>
	/// <param name="time">実行時間</param>
	/// <param name="startPos">開始状態の値</param>
	/// <param name="endPos">終了状態の値</param>
	void SetState(float time,float startValue, float endValue);

	float StartEeaging(int easingFlag);
	void Restart() {
		init = true;
		endFlg = false;
	}

	bool GetEndFlg() { return endFlg; }

private:
	float EaseIn();
	float EaseOut();
	float EaseInOut();

	float EaseInQuad();
	float EaseOutQuad();
	float EaseInOutQuad();

	float EaseInCubic();
	float EaseOutCubic();
	float EaseInOutCubic();

	float EaseInQuart();
	float EaseOutQuart();
	float EaseInOutQuart();

	float EaseInQuint();
	float EaseOutQuint();
	float EaseInOutQuint();

	float EaseInExpo();
	float EaseOutExpo();
	float EaseInOutExpo();

	float EaseInCirc();
	float EaseOutCirc();
	float EaseInOutCirc();

	float EaseInBack();
	float EaseOutBack();
	float EaseInOutBack();

	float EaseInElastic();
	float EaseOutElastic();
	float EaseInOutElastic();

	float EaseInBounce();
	float EaseOutBounce();
	float EaseInOutBounce();

private:

	float time; // 全体時間
	float rate; // 進行度
	float startValue; // 開始時の値
	float endValue; // 終了時の値
	float difference; // 開始時と終了時の値の差

	bool init = true; // 開始時間測定用

	clock_t startTime;

	bool endFlg = false;
};

