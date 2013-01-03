uniform extern float4x4 gWVP;
uniform extern float gScalar;

struct InputVS
{
	float3 pos1 : POSITION0;
	float4 diffuse1 : COLOR0;
	float3 pos2 : POSITION1;
};

struct OutputVS
{
	float4 posH : POSITION0;
	float4 diffuse : COLOR0;
};

OutputVS Main(InputVS input)
{
	OutputVS outVS = (OutputVS)0;
	float3 newPos; newPos.x = input.pos2.x; newPos.y = input.pos2.y; newPos.z = input.pos1.z + (input.pos2.z - input.pos1.z) * gScalar;
	outVS.posH = mul(float4(newPos, 1.0f), gWVP);
	outVS.diffuse = input.diffuse1;
	return outVS;
}