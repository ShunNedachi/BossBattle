#include"Particle2DShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; // �s�N�Z���V�F�[�_�ɓn���l
	output.svpos = mul(mul(mat, pos),billboard);
	output.uv = uv;
	return output;
}