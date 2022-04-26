#include"Particle2DShaderHeader.hlsli"

Texture2D<float4> tex:register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp:register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) :SV_TARGET
{
	//return tex.Sample(smp,input.uv)*color;
	float4 tempColor = {color.r,color.g,color.b,alpha};
	float4 texColor = tex.Sample(smp,input.uv) * tempColor;

	return texColor;
}