#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform int BitDepth;


vec3 BitWeight = vec3(1.0f, 1.0f, 1.0f);

void main()
{ 
	vec4 preColor = texture(screenTexture, TexCoords);
	
	//vec4 Color24Bit = preColor * 256.0f;
	
	//vec4 tempColor = vec4(0,0,0,1);
	//tempColor.r = floor(Color24Bit.r / 256) * 8;
	
	preColor.rgb = floor(preColor.rgb * BitWeight * BitDepth) / BitDepth;
    color = preColor;
}