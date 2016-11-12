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
	vec3 lightDir = normalize(lightPos - FragPos);

	float intensity = max(dot(normal, lightDir), 0.0);
	
	float AV = .3f;
	vec4 ambientColor = vec4(AV, AV, AV, 1);
	
	if (preColor.a < 1f){
		discard;
	}
	else {
		//preColor.gb *= TexCoord;
		
		//preColor.rgb *= abs(Normal.xyz);
		
		preColor.rgba *= clamp((ambientColor + intensity), 0 ,1);
		
		color = preColor;
	}

}