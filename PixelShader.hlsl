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

float4 PSMain(PSLayout layout) : SV_TARGET
{
	const float3 ambientLight = float3(0.3f, 0.3f, 0.3f);

	float3 totalLight = float3(0, 0, 0);
	float3 specular = float3(0, 0, 0), diff = float3(0, 0, 0), diffuse = float3(0, 0, 0);

	float4 g_sampleColor = g_texture.Sample(g_samplerState, layout.texCoord);

	for (int i = 0; i < MaxLights; i++)
	{
		// Calculate attenuation:
		float attenuation = length(light[i].Position.xyz - layout.fragPos);
		if (attenuation <= light[i].FallOffEnd)
		{
			// Calculate Diffuse:
			float3 norm = normalize(layout.normal);
			float3 lightDir = normalize(light[i].Position.xyz - layout.fragPos);
			diff = max(dot(norm, lightDir), 0.0f);
			diffuse += (diff * light[i].Strength.xyz * light[i].SpecularStrength) * ((light[i].FallOffEnd - attenuation) / (light[i].FallOffEnd - light[i].FallOffStart));

			// Calculate Specular:
			float3 viewDir = normalize(EyeWorldSpace.xyz - layout.fragPos);
			float3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32) / (attenuation * attenuation);
			specular += light[i].SpecularStrength * spec * light[i].Strength.xyz;

			//totalLight += ambientLight*0.2f + ComputePointLight(light[i], layout.fragPos.xyz, layout.normal.xyz, EyeWorldSpace.xyz);
		}
	}
	totalLight = (ambientLight + diffuse + specular);

	// Sampler state (texture) multiplied by the total light that we calculated
	return (g_sampleColor * float4(totalLight, 1.0f));
}