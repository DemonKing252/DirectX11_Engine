cbuffer VSConstantBuffer : register(b0)
{
	matrix World;
}

struct VSLayout
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VSLayout VSMain(float3 position : POSITION, float2 texCoord : TEXCOORD)
{
	VSLayout layout;
	
	layout.position = mul(float4(position, 1.0f), World);
	layout.texCoord = texCoord;

	return layout;
}
