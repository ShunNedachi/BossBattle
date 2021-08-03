#include "Easing.h"


void Easing::SetState(float time, float startValue, float endValue)
{
	this->time = time;
	this->startValue = startValue;
	this->endValue = endValue;
	difference = endValue - startValue;
	init = true;
}

float Easing::StartEeaging(int easingFlag)
{
	if (easingFlag & easeInFLAG)return EaseIn();
	else if (easingFlag & easeOutFLAG) return EaseOut();
	else if (easingFlag & easeInOutFLAG) return EaseInOut();
	else if (easingFlag & easeInQuadFLAG) return EaseInQuad();
	else if (easingFlag & easeOutQuadFLAG)return EaseOutQuad();
	else if (easingFlag & easeInOutQuadFLAG) return EaseInOutQuad();
	else if (easingFlag & easeInCubicFLAG)return EaseInCubic();
	else if (easingFlag & easeOutCubicFLAG)return EaseOutCubic();
	else if (easingFlag & easeInOutCubicFLAG)return EaseInOutCubic();
	else if (easingFlag & easeInQuartFLAG)return EaseInQuart();
	else if (easingFlag & easeOutQuartFLAG)return EaseOutQuart();
	else if (easingFlag & easeInOutQuartFLAG)return EaseInOutQuart();
	else if (easingFlag & easeInQuintFLAG)return EaseInQuint();
	else if (easingFlag & easeOutQuintFLAG)return EaseOutQuint();
	else if (easingFlag & easeInOutQuintFLAG)return EaseInOutQuint();
	else if (easingFlag & easeInExpoFLAG)return EaseInExpo();
	else if (easingFlag & easeOutExpoFLAG)return EaseOutExpo();
	else if (easingFlag & easeInOutExpoFLAG)return EaseInOutExpo();
	else if (easingFlag & easeInCircFLAG)return EaseInCirc();
	else if (easingFlag & easeOutCircFLAG)return EaseOutCirc();
	else if (easingFlag & easeInOutCircFLAG)return EaseInOutCirc();
	else if (easingFlag & easeInBackFLAG)return EaseInBack();
	else if (easingFlag & easeOutBackFLAG)return EaseOutBack();
	else if (easingFlag & easeInOutBackFLAG)return EaseInOutBack();
	else if (easingFlag & easeInElasticFLAG)return EaseInElastic();
	else if (easingFlag & easeOutElasticFLAG)return EaseOutElastic();
	else if (easingFlag & easeInOutElasticFLAG)return EaseInOutElastic();
	else if (easingFlag & easeInBounceFLAG)return EaseInBounce();
	else if (easingFlag & easeOutBounceFLAG)return EaseOutBounce();
	else if (easingFlag & easeInOutBounceFLAG)return EaseInOutBounce();
	else return 0;
}

float Easing::EaseIn()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}
	
	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;
	
	// 実行率
	rate = elapsedTime / time;

	float result = difference * rate * rate + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOut_Calc(float rate)
{
	return sin((rate * PI) / 2);
}
float Easing::EaseOut()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOut_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOut_Calc(float rate)
{
	return -(cos(PI * rate) - 1) / 2;
}
float Easing::EaseInOut()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOut_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInQuad_Calc(float rate)
{
	return rate * rate;
}
float Easing::EaseInQuad()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInQuad_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseOutQuad_Calc(float rate)
{
	return 1 - (1 - rate) * (1 - rate);
}
float Easing::EaseOutQuad()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutQuad_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInOutQuad_Calc(float rate)
{
	if (rate <= 0.5f)
	{
		return 2 * rate * rate;
	}
	else
	{
		return 1 - pow(-2 * rate + 2, 2) / 2;
	}

}
float Easing::EaseInOutQuad()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutQuad_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInCubic_Calc(float rate)
{
	return rate * rate * rate;
}
float Easing::EaseInCubic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInCubic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseOutCubic_Calc(float rate)
{
	return 1 - pow(1 - rate, 3);
}
float Easing::EaseOutCubic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutCubic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInOutCubic_Calc(float rate)
{
	if (rate < 0.5f)
	{
		return 4 * rate * rate * rate;
	}
	else
	{
		return 1 - pow(-2 * rate + 2, 3) / 2;
	}
}
float Easing::EaseInOutCubic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutCubic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInQuart_Calc(float rate)
{
	return rate * rate * rate * rate;
}
float Easing::EaseInQuart()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInQuart_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseOutQuart_Calc(float rate)
{
	return 1 - pow(1 - rate, 4);
}
float Easing::EaseOutQuart()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutQuart_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInOutQuart_Calc(float rate)
{
	if (rate < 0.5f)
	{
		return 8 * rate * rate * rate * rate;
	}
	else
	{
		return 1 - pow(-2 * rate + 2, 4) / 2;
	}
}
float Easing::EaseInOutQuart()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutQuart_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;

}

float EaseInQuint_Calc(float rate)
{
	return rate * rate * rate * rate * rate;
}
float Easing::EaseInQuint()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInQuint_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutQuint_Calc(float rate)
{
	return 1 - pow(1 - rate, 5);
}
float Easing::EaseOutQuint()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutQuint_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutQuint_Calc(float rate)
{
	if (rate < 0.5f)
	{
		return 16 * rate * rate * rate * rate * rate;
	}
	else
	{
		return 1 - pow(-2 * rate + 2, 5) / 2;
	}
}
float Easing::EaseInOutQuint()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutQuint_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInExpo_Calc(float rate)
{
	if (rate == 0)
	{
		return 0;
	}
	else
	{
		return pow(2, 10 * rate - 10);
	}
}
float Easing::EaseInExpo()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInExpo_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutExpo_Calc(float rate)
{
	if (rate == 1)
	{
		return 1;
	}
	else
	{
		return 1 - pow(2, -10 * rate);
	}
}
float Easing::EaseOutExpo()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutExpo_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutExpo_Calc(float rate)
{
	if (rate == 0)
	{
		return 0;
	}
	else if (rate == 1)
	{
		return 1;
	}
	else if(rate < 0.5f)
	{
		return pow(2, 20 * rate - 10) / 2;
	}
	else
	{
		return (2 - pow(2, -20 * rate + 10)) / 2;
	}
}
float Easing::EaseInOutExpo()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutExpo_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInCirc_Calc(float rate)
{
	return 1 - sqrt(1 - pow(rate, 2));
}
float Easing::EaseInCirc()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInCirc_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutCirc_Calc(float rate)
{
	return sqrt(1 - pow(rate - 1, 2));
}
float Easing::EaseOutCirc()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutCirc_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutCirc_Calc(float rate)
{
	if (rate < 0.5f)
	{
		return (1 - sqrt(1 - pow(2 * rate, 2))) / 2;
	}
	else
	{
		return (sqrt(1 - pow(-2 * rate + 2, 2)) + 1) / 2;
	}
}
float Easing::EaseInOutCirc()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutCirc_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInBack_Calc(float rate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;


	return c3 * rate * rate * rate - c1 * rate * rate;
}
float Easing::EaseInBack()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInBack_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutBack_Calc(float rate)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * pow(rate - 1, 3) + c1 * pow(rate - 1, 2);
}
float Easing::EaseOutBack()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutBack_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutBack_Calc(float rate)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;
	if (rate < 0.5)
	{
		return (pow(2 * rate, 2) * ((c2 + 1) * 2 * rate - c2)) / 2;
	}
	else
	{
		return (pow(2 * rate - 2, 2) * ((c2 + 1) * (rate * 2 - 2) + c2) + 2) / 2;
	}
}
float Easing::EaseInOutBack()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutBack_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInElastic_Calc(float rate)
{
	const float c4 = (2 * PI) / 3;
	if (rate == 0)
	{
		return 0;
	}
	else if(rate == 1)
	{
		return 1;
	}
	else
	{
		return -pow(2, 10 * rate - 10) * sin((rate * 10 - 10.75) * c4);
	}
}
float Easing::EaseInElastic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInElastic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutElastic_Calc(float rate)
{
	const float c4 = (2 * PI) / 3;
	if (rate == 0)
	{
		return 0;
	}
	else if (rate == 1)
	{
		return 1;
	}
	else
	{
		return pow(2, -10 * rate) * sin((rate * 10 - 0.75f) * c4) + 1;
	}
}
float Easing::EaseOutElastic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutElastic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutElastic_Calc(float rate)
{
	const float c5 = (2 * PI) / 4.5f;
	if (rate == 0)
	{
		return 0;
	}
	else if(rate ==1)
	{
		return 1;
	}
	else if(rate <0.5f)
	{
		return -(pow(2, 20 * rate - 10) * sin((20 * rate - 11.125f) * c5)) / 2;
	}
	else
	{
		return (pow(2, -20 * rate + 10) * sin((20 * rate - 11.125f) * c5)) / 2 + 1;
	}
}
float Easing::EaseInOutElastic()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutElastic_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutBounce_Calc(float rate);
float EaseInBounce_Calc(float rate)
{
	return 1 - EaseOutBounce_Calc(1 - rate);
}
float Easing::EaseInBounce()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInBounce_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseOutBounce_Calc(float rate)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (rate < 1 / d1)
	{
		return n1 * rate * rate;
	}
	else if(rate < 2 /d1)
	{
		return n1 * (rate -= 1.5f / d1) * rate + 0.75f;
	}
	else if (rate < 2.5f / d1)
	{
		return n1 * (rate -= 2.25f / d1) * rate + 0.9375f;
	}
	else
	{
		return n1 * (rate -= 2.625 / d1) * rate + 0.984375f;
	}
}
float Easing::EaseOutBounce()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseOutBounce_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

float EaseInOutBounce_Calc(float rate)
{
	if (rate < 0.5f)
	{
		return (1 - EaseOutBounce_Calc(1 - 2 * rate)) / 2;
	}
	else
	{
		return (1 + EaseOutBounce_Calc(2 * rate - 1)) / 2;
	}
}
float Easing::EaseInOutBounce()
{
	if (init)
	{
		// 開始時間の記録
		startTime = clock();
		init = false;
	}

	// 経過時間
	double elapsedTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;

	// 実行率
	rate = elapsedTime / time;

	float result = difference * EaseInOutBounce_Calc(rate) + startValue;

	if (elapsedTime > time)return endValue;
	return  result;
}

