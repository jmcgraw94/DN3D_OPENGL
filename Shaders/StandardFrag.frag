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
uniform int LightCount;
uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;
uniform PointLight PointLights[36];


vec4 CalculatePointLight(PointLight P, vec3 Normal, vec3 FragPos);

//float Brightness = 1.0f;
//float LightRange = 10;

float AV =  .05f;
vec4 AmbientColor = vec4(AV,AV,AV, 1);
vec4 ShadowColor = vec4(56f / 255f, 50f / 255f, 24f / 255f, 1.0f);
vec4 specColor = vec4(0,0,0,1);

void main()
{

	
	vec2 curPixel = vec2(TexCoord.x, -TexCoord.y);
	
	specColor = texture(NormalTexture, curPixel);
	vec4 imgColor = texture(MainTexture, curPixel);
	vec4 preColor = imgColor;
	vec3 normal = normalize(Normal);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
		vec4 runTotal = ShadowColor * AmbientColor;
		for (int i = 0; i < LightCount; i++){
			vec4 LightResult = CalculatePointLight(PointLights[i], normal, FragPos);
			runTotal += LightResult;
		}
		
		preColor *= runTotal;
		preColor.rgb = clamp(preColor.rgb, vec3(0,0,0), imgColor.rgb).rgb;
		
		for (int i = 0; i < LightCount; i++){
			preColor.rgb *= PointLights[i].Brightness;
		}

		//preColor.rgb = specColor.rgb;
		
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
	
	
	float Attenuation = clamp(P.Range / pow(DistFromLight, 3), 0, 1);
	
	vec4 Result = (vec4(P.Color, 1) * 
			intensity * Attenuation);
	
	return Result;
}

//preColor.gb *= TexCoord; //Adds pretty gradient
//preColor.rgb *= abs(Normal.xyz); //Illustrates face normals