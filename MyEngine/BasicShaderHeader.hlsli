cbuffer cbuff0 : register(b0)
{
	float4 color; // �F�iRGBA�j
	matrix mat; //3d�ϊ��s��
};

struct VSOutput
{
	float4 svpos:SV_POSITION; // �V�X�e���p���_���W
	float3 normal:NORMAL; // �@���x�N�g��
	float2 uv:TEXCOORD; // uv�l
};