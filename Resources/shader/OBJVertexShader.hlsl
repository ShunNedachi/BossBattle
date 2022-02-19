#include"OBJShaderHeader.hlsli"

VSOutput main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	// ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = mul(mat, pos);

	// lambert反射の計算
	output.color.rgb = dot(-lightDir, normal) * m_diffuse * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;

	// 環境反射光
	float3 ambient = m_ambient;
	// 拡散反射光
	float3 diffuse = dot(-lightDir, normal)* m_diffuse;
	// 視点座標
	const float3 eye = float3(0, 0, -20);
	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(eye - pos.xyz);
	// 反射光ベクトル
	float3 reflect = normalize(lightDir + 2 * dot(-lightDir, normal)*normal);
	// 鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess)*m_specular;
	// 全て加算する
	output.color.rgb = (ambient + diffuse + specular)*lightColor;
	output.color.a = m_alpha;

	return output;
}