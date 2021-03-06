#include"OBJShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	// sNZVF[_[Ιn·l
	VSOutput output;
	output.svpos = mul(mat, pos);

	// lambert½ΛΜvZ
	output.color.rgb = dot(-lightDir, normal) * m_diffuse * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	// Β«½Λυ
	float3 ambient = m_ambient;
	// gU½Λυ
	float3 diffuse = dot(-lightDir, normal)* m_diffuse;
	// _ΐW
	const float3 eye = float3(0, 0, -20);
	// υςx
	const float shininess = 4.0f;
	// Έ_©η_ΦΜϋόxNg
	float3 eyedir = normalize(eye - pos.xyz);
	// ½ΛυxNg
	float3 reflect = normalize(lightDir + 2 * dot(-lightDir, normal)*normal);
	// ΎΚ½Λυ
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess)*m_specular;
	// SΔΑZ·ι
	output.color.rgb = (ambient + diffuse + specular)*lightColor;
	output.color.a = m_alpha;

	return output;
}