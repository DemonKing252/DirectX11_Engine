cbuffer VSConstantBuffer : register(b0)
{
	matrix World;
	matrix Model;
	matrix View;
	matrix Proj;
}

struct VSLayout
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 fragPos : FRAG;
};

VSLayout VSMain(float3 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    VSLayout layout;
	
	layout.position = mul(float4(position, 1.0f), World);
	layout.texCoord = texCoord;
	layout.normal = mul(normal, (float3x3) Model);
	layout.fragPos = mul(float4(position, 1.0f), Model).xyz;

	return layout;
}