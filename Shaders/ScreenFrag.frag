#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform int BitDepth;
uniform vec2 ScreenSize;
uniform vec2 ViewDelta;

vec3 BitWeight = vec3(1.0f, 1.0f, 1.0f);


//float BlurSteps = 1;

void main()
{ 
	
	float BlurStep = 2f;
	float MotionStartDampen = 6f;
	float MaxBlurResult = 8f;
	float SampleDist = 16;
	
	vec2 PixelUnit = (1 / ScreenSize);
	vec4 preColor = texture(screenTexture, TexCoords);
	
	vec4 BlurColor = vec4(0);
	vec2 BlurRect = PixelUnit * SampleDist;
	
	if (SampleDist > 0 && BlurStep > 0){
		for (float y = -BlurRect.y; y < BlurRect.y; y += PixelUnit.y * BlurStep){
			for (float x = -BlurRect.x; x < BlurRect.x; x += PixelUnit.x * BlurStep){
				vec2 BlurPos = clamp(TexCoords + (vec2(x,y) * clamp(ViewDelta, -MaxBlurResult, MaxBlurResult) / MotionStartDampen), 0, 1);
				BlurColor += texture(screenTexture, BlurPos) / pow(SampleDist / BlurStep * 2,2);
			}
		}
	
		preColor = BlurColor;
	}
	
	preColor.rgb = floor(preColor.rgb * BitWeight * BitDepth) / BitDepth;
	
    color = preColor;
}