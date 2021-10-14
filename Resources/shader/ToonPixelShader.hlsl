#include "OBJShaderHeader.hlsli"

Texture2D<float4>tex:register(t0);
SamplerState smp:register(s0);

float4 main(VSOutput input) :SV_TARGET
{
	//float3 light = normalize(float3(1,-1,1));

	//float light_diffuse = saturate(dot(-light, input.normal));
	//float3 shade_color;
	//shade_color = m_ambient;
	//shade_color += m_diffuse * light_diffuse;
	//float4 texcolor = tex.Sample(smp, input.uv);
	//return float4(texcolor.rgb * shade_color, texcolor.a*m_alpha);
	
	// lambert反射の計算
	// 右、下、奥の方向を向いたライト
	float3 lightPos = { 0,100,0 };
	float3 lightdir = float3(1, -1, 0);
	lightdir = normalize(lightdir);
	// ライトの色(白)
	float3 lightcolor = float3(1, 1, 1);

	float3 eyeDir = { cameraPos.x - input.svpos.x ,cameraPos.y - input.svpos.y , cameraPos.z - input.svpos.z };
	float intensity = saturate(dot(normalize(input.normal), normalize(cameraPos + lightPos)));

	float specular = step(0.5, pow(intensity, 50));

	float3 ambient = m_ambient;
	float3 diffuse = m_diffuse;

	float3 ads = diffuse + specular;

	float4 colorBlack = (0, 0, 0, 1);
	float3 toonColor = step(0.5, intensity) * ads + (1 - step(0.5, intensity)) * colorBlack;

	float4 texcolor = tex.Sample(smp,input.uv);

	float4 output;

	output.rgb = toonColor;
	output.a = m_alpha;
	
	return output * texcolor;
}