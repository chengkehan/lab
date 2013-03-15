struct InputVS
{
	float3 pos0 : POSITION0;
	float4 diffuse0 : COLOR0;
};

struct OutputVS
{
	float4 pos0 : POSITION0;
	float4 diffuse0 : COLOR0;
};

OutputVS Main(InputVS input)
{
	OutputVS outVS = (OutputVS)0;
	//input.pos0.x -= 1.0f;
	//input.pos0.y += 1.0f;
	outVS.pos0 = float4(input.pos0, 1.0f);
	outVS.diffuse0 = input.diffuse0;
	return outVS;
}