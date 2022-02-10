cbuffer cbuff0:register(b0)
{
	matrix mat;
	float3 cameraPos;
	float b0pad;
	float3 color;
}

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient :packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
}

cbuffer cbuff2:register(b2)
{
	float3 lightPos;
	float lightPad;
	float3 lightDir;
	float lightPad2;
	float3 lightColor;
}

struct VSOutput
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
	float4 color:COLOR;
	float2 uv:TEXCOORD;
};