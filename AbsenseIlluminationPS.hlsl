#define PointLightCount 2
struct PointLight
{
	// 4 bytes for each floating point number

	// 16 bytes
	float3 Position;
	float FallOffStart;

	// 16 bytes
	float3 Strength;
	float SpecularStrength;
	
	// 16 bytes
	float3 Direction;
	float FallOffEnd;
};

cbuffer PSConstantBuffer : register(b0)
{
	float4 EyeWorldSpace; // Eye Position (World Space)
    float4 Color;
    
    PointLight light[PointLightCount];
}

struct PSLayout
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 fragPos : FRAG;
};

float4 PSMain(PSLayout layout) : SV_TARGET
{
	return clamp(Color, 0.0f, 1.0f);
}