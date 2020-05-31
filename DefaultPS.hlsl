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
	float4 EyeWorldSpace;		// Eye Position (World Space)
    
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

float3 ComputePointLight(PointLight light, PSLayout layout, float attenuation)
{
	float3 specular = float3(0, 0, 0), diff = float3(0, 0, 0), diffuse = float3(0, 0, 0);

	// Calculate Diffuse:
	float3 norm = normalize(layout.normal);
	float3 lightDir = normalize(light.Position.xyz - layout.fragPos);
	diff = max(dot(norm, lightDir), 0.0f);
    diffuse = (diff * light.Strength.xyz * light.SpecularStrength) * ((light.FallOffEnd - attenuation) / (light.FallOffEnd - light.FallOffStart));

	// Calculate Specular:
	float3 viewDir = normalize(EyeWorldSpace.xyz - layout.fragPos);
	float3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32) / (attenuation * attenuation);
	specular = light.SpecularStrength * spec * light.Strength.xyz;

	return diffuse + specular;
}
float4 PSMain(PSLayout layout) : SV_TARGET
{
	const float3 ambientLight = float3(0.2f, 0.2f, 0.2f);

	float3 totalLight = float3(0, 0, 0);

	float4 g_sampleColor = g_texture.Sample(g_samplerState, layout.texCoord);

	for (int i = 0; i < MaxLights; i++)
	{
		float attenuation = length(light[i].Position.xyz - layout.fragPos);
        
		if (attenuation <= light[i].FallOffEnd)
			totalLight += ComputePointLight(light[i], layout, attenuation);
	}
	totalLight += ambientLight;

	// Sampler state (texture) multiplied by the total light that we calculated
    return (g_sampleColor * float4(totalLight, 1.0f));
}
/*
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
	float4 EyeWorldSpace;		// Eye Position (World Space)
    
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

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // Linear falloff.
    return saturate((falloffEnd - d) / (falloffEnd - falloffStart));
}
float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}
float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye)
{
    const float m = 256.0f;
    float3 halfVec = normalize(toEye + lightVec);

    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(1.0f, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor * roughnessFactor;

    // Our spec formula goes outside [0,1] range, but we are 
    // doing LDR rendering.  So scale it down a bit.
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (float3(5, 5, 5) + specAlbedo) * lightStrength;
}

float3 ComputePointLight(PointLight L, float3 pos, float3 normal, float3 toEye)
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
    float3 lightStrength = L.Strength * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(d, L.FallOffStart, L.FallOffEnd);
    lightStrength *= att;

    return BlinnPhong(lightStrength, lightVec, normal, toEye);
}
float4 PSMain(PSLayout layout) : SV_TARGET
{
	const float3 ambientLight = float3(0.2f, 0.2f, 0.2f);

	float3 totalLight = float3(0, 0, 0);

	float4 g_sampleColor = g_texture.Sample(g_samplerState, layout.texCoord);

	for (int i = 0; i < MaxLights; i++)
	{
        totalLight += ComputePointLight(light[i], layout.position.xyz, layout.normal, EyeWorldSpace.xyz);
    }
	//totalLight += ambientLight;

	// Sampler state (texture) multiplied by the total light that we calculated
    return (g_sampleColor * float4(totalLight, 1.0f));
}
*/