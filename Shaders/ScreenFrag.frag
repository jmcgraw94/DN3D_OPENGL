#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform int BitDepth;

vec3 BitWeight = vec3(1.0f, 1.0f, 1.0f);

void main()
{ 
	vec4 preColor = texture(screenTexture, TexCoords);
	
	preColor.rgb = floor(preColor.rgb * BitWeight * BitDepth) / BitDepth;
	
    color = preColor;
}