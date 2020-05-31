#define MaxLights 2
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
    
	PointLight light[MaxLights];
}


struct PSLayout
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 fragPos : FRAG;
};
Texture2D g_texture : TEXTURE : register(t0);
SamplerState g_samplerState : SAMPLE : register(s0);

float4 PSMain(PSLayout layout) : SV_TARGET
{
	float4 ambientLight = float4(0.2f, 0.2f, 0.2f, 1.0f);

	return clamp(float4(light[0].Strength, 1.0f), 0.0f, 2.0f);
}