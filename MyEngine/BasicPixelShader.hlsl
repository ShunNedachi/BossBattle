#include"BasicShaderHeader.hlsli"

//cbuffer cbuff:register(b0)
//{
//	float4 color;
//}

Texture2D<float4> tex:register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp:register(s0);		// 0番スロットに設定されたサンプラー


float4 PSmain(VSOutput input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv))* color;


	float3 light = normalize(float3(1,-1,1)); // 右下奥　向きのライト
	float diffuse = saturate(dot(-light, input.normal)); // diffuseを[0,1]の範囲にclampする
	float brightness = diffuse + 0.3f; // アンビエント項を0.3として計算
	float4 texcolor = (tex.Sample(smp, input.uv));

	return float4(texcolor.r * brightness,texcolor.g * brightness,texcolor.b * brightness
		,texcolor.a) * color;
	// return float4(brightness, brightness, brightness, 1); // 輝度をrgbに代入して出力

}