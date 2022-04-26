cbuffer cbuff0:register(b0)
{
	float3 color; // �F(RGB)
	float alpha;
	matrix mat; // 3d�s��ϊ�
	matrix billboard; // �r���{�[�h�s��
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�ւ̂��Ƃ�Ɏg�p����\����
struct VSOutput
{
	float4 svpos:SV_POSITION; // �V�X�e���p���_���W
	float2 uv:TEXCOORD; // uv�l
};