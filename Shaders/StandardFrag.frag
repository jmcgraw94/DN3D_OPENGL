#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;

uniform vec3 lightPos;
uniform vec3 lightColor;

struct PointLight {    
    vec3 Position;
}; 

void main()
{
	vec2 curPixel = vec2(TexCoord.x, -TexCoord.y);
	vec4 normalColor = texture(NormalTexture, curPixel);

	vec4 imgColor = texture(MainTexture, curPixel);
	vec4 preColor = imgColor;
	
	vec3 normal = normalize(Normal);
		
	vec3 LightToFragVec = lightPos - FragPos;
	vec3 lightDir = normalize(LightToFragVec);
	
	float intensity = max(dot(normal, lightDir), 0.0);
	float Brightness = 1.125f;
	
	vec3 ShadowColor = vec3(56f / 255f, 50f / 255f, 24f / 255f);
	
	float AV =  .3f;
	vec4 ambientColor = vec4(AV,AV,AV, 1);

	float LightRange = 15;
	float DistFromLight = length(LightToFragVec);
	float Attenuation = clamp(LightRange / pow(DistFromLight, 2), 0, Brightness);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
				
		preColor.rgba *= clamp((ambientColor +  (vec4(lightColor, 1) * 
			intensity * Attenuation)), 0 , Brightness);
		
		color = preColor;
	}

}

//preColor.gb *= TexCoord; //Adds pretty gradient
//preColor.rgb *= abs(Normal.xyz); //Illustrates face normals