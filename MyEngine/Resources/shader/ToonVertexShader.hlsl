#include"OBJShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{


	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	//// lambert���˂̌v�Z
	//output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	////output.color.a = m_alpha;


	//output.color.a = m_alpha;
	output.uv = uv;

	return output;
}