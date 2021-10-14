#include"OBJShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{


	// ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	//// lambert反射の計算
	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	////output.color.a = m_alpha;


	//output.color.a = m_alpha;
	output.uv = uv;

	return output;
}