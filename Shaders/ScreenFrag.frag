#version 330 core
//Inputs
in vec2 TexCoords;

//Outputs
out vec4 color;

//Uniforms
uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform int BitDepth;
uniform vec2 ScreenSize;
uniform vec2 ViewDelta;

//Local Variables
vec3 BitWeight = vec3(1.0f, 1.0f, 1.0f);

float MotionStartDampen = 7f;
float MaxBlurResult = 16f;

float BlurStep = 1.75f;
float SampleDist = 8;

vec4 preColor;

//Functions
void MotionBlur();

void main()
{ 
	preColor = texture(screenTexture, TexCoords);

	MotionBlur();
	preColor.rgb = floor(preColor.rgb * BitWeight * BitDepth) / BitDepth;
	
    color = preColor;
}

void MotionBlur(){
	vec2 PixelUnit = (1 / ScreenSize);
	vec4 BlurAverage = vec4(0);
	vec2 BlurRect = PixelUnit * SampleDist;
	
	if (SampleDist > 0 && BlurStep > 0){
		for (float y = -BlurRect.y; y < BlurRect.y; y += PixelUnit.y * BlurStep){
			for (float x = -BlurRect.x; x < BlurRect.x; x += PixelUnit.x * BlurStep){
				vec2 BlurPos = clamp(TexCoords + (vec2(x,y) * clamp(ViewDelta, -MaxBlurResult, MaxBlurResult) / MotionStartDampen), 0, 1);
				BlurAverage += texture(screenTexture, BlurPos) / pow(SampleDist / BlurStep * 2,2);
			}
		}
	
		preColor = BlurAverage;
	}
}