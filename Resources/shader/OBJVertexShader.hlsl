#include"OBJShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	output.svpos = mul(mat, pos);

	// lambert���˂̌v�Z
	output.color.rgb = dot(-lightDir, normal) * m_diffuse * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	// �����ˌ�
	float3 ambient = m_ambient;
	// �g�U���ˌ�
	float3 diffuse = dot(-lightDir, normal)* m_diffuse;
	// ���_���W
	const float3 eye = float3(0, 0, -20);
	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(eye - pos.xyz);
	// ���ˌ��x�N�g��
	float3 reflect = normalize(lightDir + 2 * dot(-lightDir, normal)*normal);
	// ���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess)*m_specular;
	// �S�ĉ��Z����
	output.color.rgb = (ambient + diffuse + specular)*lightColor;
	output.color.a = m_alpha;

	return output;
}