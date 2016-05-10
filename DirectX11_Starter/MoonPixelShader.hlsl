Texture2D myTexture: register(t0);
Texture2D normalMap: register(t1);
SamplerState mySampler: register(s0);


cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
	float4 textureColor;
};

cbuffer CameraBuffer : register(b1)
{
	float3 cameraPosition;
	float padding;
};

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		 : SV_POSITION;
	float3 normal		 : TEXCOORD0;
	float2 uv			 : TEXCOORD1;
	float3 viewDirection : TEXCOORD2;
	float3 tangent		 : TANGENT;
	float3 binormal		 : BINORMAL;
};

float4 lerp(float4 a, float4 b, float x)
{
	return (1 - x)*a + b*x;
}

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = lerp(myTexture.Sample(mySampler, input.uv), diffuseColor, 0.5);
	float3 normal = normalize(((2.0f * (normalMap.Sample(mySampler, input.uv).rgb)) - 1.0f));

	float3 LightDir = normalize(lightDirection);
	float3 viewDir = normalize(input.viewDirection);
	float4 diff = saturate(dot(LightDir, normal));

	float3 reflection = reflect(LightDir, normal);
	float4 specular = pow(saturate(dot(reflection, viewDir)), specularPower);
	color = ambientColor + (color *diff) + (specular * specularColor);
	float angle = 0;
	angle = dot(input.normal, normalize(viewDir));
	color.w = min(pow(angle, 2)*2, 1.0);
	float t = (1 - (pow(angle, 4)))/2;
	color.rgb += float3(t,t,t);
	return saturate(color);
}
