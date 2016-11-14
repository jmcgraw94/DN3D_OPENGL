#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	vec2 texRev = vec2(TexCoord.x, -TexCoord.y);
	vec4 imgColor = texture(ourTexture, texRev);
	vec4 preColor = imgColor;
	
	vec3 normal = normalize(Normal);
	vec3 LightToFragVec = lightPos - FragPos;
	vec3 lightDir = normalize(LightToFragVec);

	float intensity = max(dot(normal, lightDir), 0.0);
	
	vec3 ShadowColor = vec3(56f / 255f, 50f / 255f, 24f / 255f);
	
	float AV =  .3f;
	vec4 ambientColor = vec4(AV,AV,AV, 1);

	float LightRange = 15;
	float DistFromLight = length(LightToFragVec);
	float Attenuation = clamp(LightRange / pow(DistFromLight, 2), 0, 1);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
		//preColor.gb *= TexCoord;
		
		//preColor.rgb *= abs(Normal.xyz);
		
		preColor.rgba *= clamp((ambientColor +  (vec4(lightColor, 1) * intensity * Attenuation)), 0 ,1);
		
		color = preColor;
	}

}