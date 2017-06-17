#version 330 core

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fs_in;

uniform vec3 viewPos;

//material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
//uniform sampler2D roughnessMap;
//uniform sampler2D aoMap;

//lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

out vec4 FargColor;

const float PI = 3.14159265359;

vec3 getNormalFromMap();

float distributionGGX(vec3 N, vec3 H, float roughness);

float  geometrySchlickGGX(float NdotV, float roughness);

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0);

void main()
{
	vec3 albedo = pow(texture2D(albedoMap, fs_in.texcoord).rgb, vec3(2.2));	//转化到线性空间
	//vec3 albedo = texture2D(albedoMap, fs_in.texcoord).rgb;
	float metallic = texture2D(metallicMap, fs_in.texcoord).r;
	float roughness = 1- metallic;//texture2D(roughnessMap, fs_in.texcoord).r;
	float ao = 1.0f;//texture2D(aoMap, fs_in.texcoord).r;

	vec3 N = normalize(fs_in.normal);		//规则化法向量
	vec3 V = normalize(viewPos - fs_in.position);		//该像素和眼睛之间的方向向量
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	
	//反射方程
	vec3 L0 = vec3(0.0);
	for(int i = 0; i < 4; ++i)
	{
		//计算每条光线的辐射率
		vec3 L = normalize(lightPositions[i] - fs_in.position);
		vec3 H = normalize(V+ L);
		float distance = length(lightPositions[i] - fs_in.position);
		float attenuation = 1.0f / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;
		
		//cook-torrance BRDF
		float NDF = distributionGGX(N, H, roughness);
		float G = geometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0f), F0);
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;
		
		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.001;
		
		vec3 specular = nominator / denominator;
		
		float NdotL = max(dot(N, L), 0.0f);
		L0 += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	
	vec3 ambient = vec3(0.03) * albedo *ao;
	vec3 color = ambient + L0;
	
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));
	
	//HDR
	color = color / (color + vec3(1.0));
	//gamma correct
	color = pow(color, vec3(1.0 / 2.2));
	
	FargColor = vec4(vec3(color), 1.0f);
}

vec3 getNormalFromMap()
{
	vec3 tangentNormal = texture2D(normalMap, fs_in.texcoord).xyz * 2.0 - 1.0;
	
	vec3 Q1 = dFdx(fs_in.position);
	vec3 Q2 = dFdy(fs_in.position);
	vec2 st1 = dFdx(fs_in.texcoord);
	vec2 st2 = dFdy(fs_in.texcoord);
	
	vec3 N = normalize(fs_in.normal);
	vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);
	
	return normalize(TBN * tangentNormal);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0f);
	float NdotH2 = NdotH * NdotH;
	
	float nom = a2;
	float dnom = (NdotH * (a2 - 1.0) + 1.0f);
	dnom = PI * dnom * dnom;
	
	return nom / dnom;
}

float  geometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0f;
	
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	denom = PI * denom * denom;
	
	return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L) ,0.0f);
	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f);
}