cbuffer cbuff0:register(b0)
{
	float4 color; // 色(RGBA)
	matrix mat; // 3d行列変換
};

// 頂点シェーダーからピクセルシェーダへのやりとりに使用する構造体
struct VSOutput
{
	float4 svpos:SV_POSITION; // システム用頂点座標
	float2 uv:TEXCOORD; // uv値
};