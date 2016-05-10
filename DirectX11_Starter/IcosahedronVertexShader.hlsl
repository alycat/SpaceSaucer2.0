struct GSInput
{
	float4 pos : SV_POSITION;
	float4 direction : COLOR;
};

GSInput main(GSInput pos)
{
	return pos;
}