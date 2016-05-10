cbuffer perModel : register(b0)
{
	float4 ambient		: COLOR0;
	float4 diffuse		: COLOR1;
	float3 lightDirection	: TEXCOORD0;
	float specularPower : TEXCOORD1;
	float4 specular		: COLOR2;
	float highlight : COLOR3;
};

cbuffer resolution: register(b1)
{
	float3 camPos;
	float buffer;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: TEXCOORD0;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 lightDir = normalize(lightDirection);
	float3 viewDir = normalize(camPos);
	float3 normal = normalize(input.normal);
	float diff = saturate(dot(-lightDir, normal));
	float3 reflect = normalize(2 * diff*normal - lightDir);
	float spec = pow(saturate(dot(reflect, viewDir)), specularPower);
	float angle = dot(normal, normalize(-normalize(input.position.xyz) + viewDir));
	if (angle <= 0.1)
		return float4(0, 1, 0, 1);
	return saturate(ambient + diffuse * diff + specular * spec);
}