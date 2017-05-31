#version 330 core

in VS_OUT
{
	vec3 position;
	vec2 texcoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
}fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

out vec4 color;

vec2 parallaxMap(vec2 texcoord, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentPosition);
	vec2 texcoord = parallaxMap(fs_in.texcoord, viewDir);
	
	vec3 texColor = texture2D(diffuseMap, texcoord).rgb;
	vec3 normal = texture2D(normalMap, texcoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec3 ambient = 0.1 * texColor;
	
	vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentPosition);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * texColor;
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec * vec3(0.2f);
	
	vec3 result = ambient + diffuse + specular;
	
	color = vec4(result, 1.0f);
}

vec2 parallaxMap(vec2 texcoord, vec3 viewDir)
{
	//number of depth layers
	const float numLayers = 10.0f;
	//calculate the size of each layer
	float layerDepth = 1.0f / numLayers;
	//depth or current layer
	float currentLayerDepth = 0.0f;
	//the amount to shift the texture coordinates per layer(from vector p)
	vec2 P = viewDir.xy * heightScale;
	vec2 deltaTexCoords = P / numLayers;
	
	//get initial values
	vec2 currentTexCoords = texcoord;
	float currentDepthMapValue = texture2D(depthMap, currentTexCoords).r;
	
	while(currentLayerDepth < currentDepthMapValue)
	{
		//shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		//get depthmap value at current texture coordinates
		currentDepthMapValue = texture2D(depthMap, currentTexCoords).r;
		//get depth of next layer
		currentLayerDepth += layerDepth;
	}
	
	return currentTexCoords;
}