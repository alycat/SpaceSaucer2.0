Texture2D myTexture: register(t0);
SamplerState mySampler: register(s0);

cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer resolution: register(b1)
{
	float3 resolution;
	float empty;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: TEXCOORD0;
	float2 uv			: TEXCOORD1;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,1,1,1);
}