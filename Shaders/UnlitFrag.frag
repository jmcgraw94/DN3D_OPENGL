#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;

uniform vec3 Tint;

void main()
{
	vec2 texRev = vec2(TexCoord.x, -TexCoord.y);
	vec4 imgColor = texture(MainTexture, texRev);
	vec4 preColor = imgColor * vec4(Tint.rgb, 1.0f);
	
	
	if (preColor.a < 1f){
		discard;
	}
	else {
	
		color = preColor;
	}

}