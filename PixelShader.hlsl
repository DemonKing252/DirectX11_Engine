cbuffer PSConstantBuffer : register(b0)
{
	float4 Color;
	float4 EyeWorldSpace;
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

struct Light
{
	float Strength;
	float3 Position;
	float3 Color;
	float FallOffStart;
	float FallOffEnd;
};

//struct Light
//{
//	float3 Strength;
//	float FalloffStart; // point/spot light only
//	float3 Direction;   // directional/spot light only
//	float FalloffEnd;   // point/spot light only
//	float3 Position;    // point light only
//	float SpotPower;    // spot light only
//};

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
	// Linear falloff.
	return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);

	return reflectPercent;
}
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye)
{
	const float m = 256.0f;
	float3 halfVec = normalize(toEye + lightVec);

	float roughnessFactor = (m + 8.0f)*pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 fresnelFactor = SchlickFresnel(1.0f, halfVec, lightVec);

	float3 specAlbedo = fresnelFactor * roughnessFactor;

	// Our spec formula goes outside [0,1] range, but we are 
	// doing LDR rendering.  So scale it down a bit.
	specAlbedo = specAlbedo / (specAlbedo + 1.0f);

	return (float3(1.0f, 1.0f, 1.0f) + specAlbedo) * lightStrength;
}
float3 ComputePointLight(Light L, float3 pos, float3 normal, float3 toEye)
{
	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;

	// The distance from surface to light.
	float d = length(lightVec);

	// Range test.
	if (d > L.FallOffEnd)
		return 0.0f;

	// Normalize the light vector.
	lightVec /= d;

	// Scale light down by Lambert's cosine law.
	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 lightStrength = L.Color * ndotl;

	// Attenuate light by distance.
	float att = CalcAttenuation(d, L.FallOffStart, L.FallOffEnd);
	lightStrength *= att;

	return BlinnPhong(lightStrength, lightVec, normal, toEye);
}

float4 PSMain(PSLayout layout) : SV_TARGET
{
	Light g_pointLight;
	const float3 ambientLight = float3(0.3f, 0.3f, 0.3f);

	// light information
	g_pointLight.Position = float3(-2.0f, 0.0f, 0.0f);
	g_pointLight.Color = float3(1.5f, 1.5f, 1.5f);
	g_pointLight.Strength = 1.0f;
	g_pointLight.FallOffStart = 0.1f;
	g_pointLight.FallOffEnd = 15.0f;

	float3 totalLight = float3(0, 0, 0);

	float4 g_sampleColor = g_texture.Sample(g_samplerState, layout.texCoord);

	// Calculate distance for Attenuation strength:
	float distance = length(g_pointLight.Position - layout.fragPos);

	// Calculate Diffuse:
	float3 norm = normalize(layout.normal);
	float3 lightDir = normalize(g_pointLight.Position - layout.fragPos);
	float diff = max(dot(norm, lightDir), 0.0f);


	totalLight = ambientLight + (diff * g_pointLight.Color * g_pointLight.Strength) * ((g_pointLight.FallOffEnd - distance) / (g_pointLight.FallOffEnd - g_pointLight.FallOffStart));
	//totalLight = ambientLight + ComputePointLight(g_pointLight, layout.fragPos, norm, float3(0.0f, 0.0f, 3.0f));

	// Sampler state (texture) multiplied by the total light that we calculated
	return (g_sampleColor * float4(totalLight, 1.0f));
}