Texture2D myTexture: register(t0);
SamplerState mySampler: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: TEXCOORD0;
	float2 uv			: TEXCOORD1;
};

cbuffer asteroidBuffer: register(b0)
{
	float age;
	float3 position;
	float3 acceleration;
};

half noise(int x, int y){
	int n = x + y * 5;
	n = (n << 13) ^ n;
	return saturate((1.0 - ((n * n* (n * n * 157311 + 789221) + 1376312589) & 2147483645) / 1073741824.0));
}

half smoothedNoise(half x, half y){
	half corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16;
	half side = (noise(x - 1, y) + noise(x, y - 1) + noise(x + 1, y) + noise(x, y + 1)) / 8;
	half center = noise(x, y) / 4;
	return corners + side + center;
}


half gradientNoise(int x, int y){
	return saturate(pow(noise(x, y), 1.1));
}

float2 twirl(float dist, float angle) {
	float percent = (0.5 - dist) / 0.5;
	float theta = percent*percent*angle*8.0;
	float y = sin(theta);
	float x = cos(theta);
	return float2(x, y);
}

half interpolatedNoise(half x, half y){
	int _x = (int)x;
	half frac_x = x - _x;
	int _y = (int)y;
	half frac_y = y - _y;

	half v[4];
	v[0] = gradientNoise(x, y);
	v[1] = gradientNoise(x + 1, y);
	v[2] = gradientNoise(x, y + 1);
	v[3] = gradientNoise(x + 1, y + 1);

	half i1 = lerp(v[0], v[1], frac_x);
	half i2 = lerp(v[2], v[3], frac_x);

	return lerp(i1, i2, frac_y);
}

half octave(half x, half y, half pers, int octave){
	half freq = 1.0f;
	half amp = 1.0f;
	half max = 0.0f;
	half total = 0.0f;
	for (int i = 0; i < octave; ++i){
		total += interpolatedNoise(x * freq, y * freq) * amp;
		max += amp;
		amp = pow(pers, (half) i);
		freq = 2 ^ i;
	}
	//total /= max;
	return saturate(total);
}

half3 color(half t){
	half r = (1 + (sin(t * 10))) / 2;
	half g = (1 + (cos(t * 20)))/2;
	half b = (1 + (cos(t * 20)))/2;
	return saturate(half3(r, g, b));
}

half4 main(VertexToPixel input) : SV_TARGET
{
	int oct = 3;
	//oct = 1;
	half pers = 0.75;
	float2 uv = input.uv - float2(0.5, 0.5);
	float2 cs = twirl(length(uv), ((1 + sin(age/5)) * 2) - 5);
	uv = float2(dot(uv, float2(cs.x, -cs.y)), dot(uv, cs));
	half x = input.position.x;
	half y = input.position.y;
	float denom = (2 + sin(age/5))*50;// +abs(tan(age / 20));
	float tone = octave(((x*uv.x*uv.x))/(denom), ((y*uv.y*uv.y))/(denom), 0.1, 3)/2.5;
	float3 newColor = color(tone);
	float cloud = octave((x/100) + age/10, y/100, 0.75, 7)/5;
	float4 totalColor = float4(newColor, tone/2);
	totalColor += float4(cloud, cloud, cloud, cloud/5);
	float value = (noise((x * uv.x)*10, (y * uv.y)*10));
	if (value > 0.95)
		totalColor += float4(value * value, value * value, value, value/100);
	return saturate(totalColor);
}