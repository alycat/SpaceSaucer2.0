Texture2D myTexture: register(t0);
SamplerState mySampler: register(s0);

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
	float4 highlightColor;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: TEXCOORD0;
	float2 uv			: TEXCOORD1;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = myTexture.Sample(mySampler, input.uv);
	color *= diffuseColor;
	return saturate(color);
}