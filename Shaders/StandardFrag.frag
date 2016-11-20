#version 330 core
#define NUM_POINT_LIGHTS 1

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct PointLight {    
    vec3 Position;
	vec3 Color;
};


uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;
uniform PointLight PointLights[NUM_POINT_LIGHTS];

vec4 CalculatePointLight(PointLight P, vec3 Normal, vec3 FragPos);

void main()
{
	vec2 curPixel = vec2(TexCoord.x, -TexCoord.y);
	vec4 normalColor = texture(NormalTexture, curPixel);

	vec4 imgColor = texture(MainTexture, curPixel);
	vec4 preColor = imgColor;
	
	vec3 normal = normalize(Normal);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
		//preColor.rgba *= CalculateAmbientLight();
		for (int i = 0; i < NUM_POINT_LIGHTS; i++){
			preColor.rgba *= CalculatePointLight(PointLights[i], normal, FragPos);
		}
		
		
		color = preColor;
	}

}

vec4 CalculatePointLight(PointLight P, vec3 _normal, vec3 FragPos){
	vec3 LightToFragVec = P.Position - FragPos;
	vec3 lightDir = normalize(LightToFragVec);
	
	float DistFromLight = length(LightToFragVec);
	float LightRange = 5;
		
	
	
	float intensity = max(dot(_normal, lightDir), 0.0);
	float Brightness = 1.125f;
	
	//vec3 ShadowColor = vec3(56f / 255f, 50f / 255f, 24f / 255f);
	
	float AV =  .3f;
	vec4 ambientColor = vec4(AV,AV,AV, 1);
	
	float Attenuation = clamp(LightRange / pow(DistFromLight, 2), 0, Brightness);
	
	vec4 Result = clamp((ambientColor +  (vec4(P.Color, 1) * 
			intensity * Attenuation)), 0 , Brightness);
	
	return Result;
}
//preColor.gb *= TexCoord; //Adds pretty gradient
//preColor.rgb *= abs(Normal.xyz); //Illustrates face normals