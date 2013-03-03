uniform extern float4x4 worldMatrix;
uniform extern float4x4 vpMatrix;
uniform extern float radian;

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
	worldMatrix._11 = cos(radian); worldMatrix._13 = -sin(radian);
	worldMatrix._31 = sin(radian); worldMatrix._33 = cos(radian);
	float4x4 wvpMatrix = mul(worldMatrix, vpMatrix);
	outVS.pos0 = mul(float4(input.pos0, 1.0f), wvpMatrix);
	outVS.diffuse0 = input.diffuse0;
	return outVS;
}