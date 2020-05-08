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

VSLayout VSMain(float4 position : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
	VSLayout layout;
	
	layout.fragPos = mul(position.xyz, (float3x3)Model);

	//matrix MVP = Model * View * Proj;
	//MVP = transpose(MVP);

	layout.position = mul(position, World);

	layout.texCoord = texCoord;
	layout.normal = mul(normal, (float3x3)Model);

	return layout;
}
