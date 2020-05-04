struct PSLayout
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};
Texture2D g_texture : TEXTURE : register(t0);
SamplerState g_samplerState : SAMPLE : register(s0);

float4 PSMain(PSLayout layout) : SV_TARGET
{
	float4 g_sampleColor = g_texture.Sample(g_samplerState, layout.texCoord);

	return g_sampleColor;
}