Texture2D myTexture: register(t0);
SamplerState mySampler: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 pos			: TEXCOORD0;
	float2 uv			: TEXCOORD1;
};

float4 main(in VertexToPixel input) : SV_TARGET
{
	float2 uv = input.uv - float2(0.5, 0.5);
	float4 color = myTexture.Sample(mySampler, input.uv);
	float factor = 1000;
	return float4(color.rgb, abs(color.a/(factor * max(uv.x * uv.y, 1/factor))));
}