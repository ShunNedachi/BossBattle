#include"Particle2DShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダに渡す値
	output.svpos = mul(mul(mat, pos),billboard);
	output.uv = uv;
	return output;
}