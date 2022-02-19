#include"SpriteShaderHeader.hlsli"

Texture2D<float4> tex:register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 PSmain(VSOutput input) :SV_TARGET
{
	//return tex.Sample(smp,input.uv)*color;
	float4 tempColor = {color.r,color.g,color.b,alpha};
	float4 texColor = tex.Sample(smp,input.uv) * tempColor;

	return texColor;
}