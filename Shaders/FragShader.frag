#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	vec2 texRev = vec2(-TexCoord.x, TexCoord.y);
	vec4 preColor = texture(ourTexture, texRev);
	
	if (preColor.a < .5f)
		discard;
	else
	{
		preColor.gb *= TexCoord;
		color = preColor;
	}
}