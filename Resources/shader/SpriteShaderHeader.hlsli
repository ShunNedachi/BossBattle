cbuffer cbuff0:register(b0)
{
	float3 color; // 色(RGB)
	float alpha;
	float pad1;
	float pad2;
	matrix mat; // 3d行列変換
};

// 頂点シェーダーからピクセルシェーダへのやりとりに使用する構造体
struct VSOutput
{
	float4 svpos:SV_POSITION; // システム用頂点座標
	float2 uv:TEXCOORD; // uv値
};