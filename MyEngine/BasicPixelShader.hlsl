#include"BasicShaderHeader.hlsli"

//cbuffer cbuff:register(b0)
//{
//	float4 color;
//}

Texture2D<float4> tex:register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);		// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 PSmain(VSOutput input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv))* color;


	float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal)); // diffuse��[0,1]�͈̔͂�clamp����
	float brightness = diffuse + 0.3f; // �A���r�G���g����0.3�Ƃ��Čv�Z
	float4 texcolor = (tex.Sample(smp, input.uv));

	return float4(texcolor.r * brightness,texcolor.g * brightness,texcolor.b * brightness
		,texcolor.a) * color;
	// return float4(brightness, brightness, brightness, 1); // �P�x��rgb�ɑ�����ďo��

}