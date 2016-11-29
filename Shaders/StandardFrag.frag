#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct PointLight {    
    vec3 Position;
	vec3 Color;
	float Range;
	float Brightness;
};


uniform int DSL = 0; //Double Side Lighting
uniform int DistanceLighting = 0;
//uniform int LightCount;
uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;
uniform PointLight PointLights[2];


vec4 CalculatePointLight(PointLight P, vec3 Normal, vec3 FragPos);

//float Brightness = 1.0f;
//float LightRange = 10;

float AV =  .2f;
vec4 AmbientColor = vec4(AV,AV,AV, 1);

void main()
{
	int LightCount = 2;
	
	vec2 curPixel = vec2(TexCoord.x, -TexCoord.y);
	//vec4 normalColor = texture(NormalTexture, curPixel);

	vec4 imgColor = texture(MainTexture, curPixel);
	vec4 preColor = imgColor;
	
	vec3 normal = normalize(Normal);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
		for (int i = 0; i < LightCount; i++){
			vec4 LightResult = CalculatePointLight(PointLights[i], normal, FragPos);
			
			LightResult.rgb = clamp(LightResult.rbg, 0, PointLights[i].Brightness);
			
			preColor.rgb *= (AmbientColor.rgb + (LightResult.rgb));
			
			preColor.rgb = clamp(preColor.rbg, 0, PointLights[i].Brightness);
		}
		
		
		color = preColor;
	}

}

vec4 CalculatePointLight(PointLight P, vec3 _normal, vec3 FragPos){
	vec3 LightToFragVec = P.Position - FragPos;
	vec3 lightDir = normalize(LightToFragVec);
	
	float DistFromLight = length(LightToFragVec);
	
	float intensity = 0;
	if (DSL == 1)
		intensity = abs(dot(_normal, lightDir));
	else
		intensity = max(dot(_normal, lightDir), 0.0);
	if (DistanceLighting == 1)
		intensity = .8f;
	//float Brightness = 1.125f;
	//vec3 ShadowColor = vec3(56f / 255f, 50f / 255f, 24f / 255f);
	
	float Attenuation = clamp(P.Range / pow(DistFromLight, 2), 0, P.Brightness);
	
	vec4 Result = (vec4(P.Color, 1) * 
			intensity * Attenuation);
	
	return Result;
}
//preColor.gb *= TexCoord; //Adds pretty gradient
//preColor.rgb *= abs(Normal.xyz); //Illustrates face normals