uniform extern float4x4 wvpMatrix;

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
	outVS.pos0 = mul(float4(input.pos0, 1.0f), wvpMatrix);
	outVS.diffuse0 = input.diffuse0;
	return outVS;
}