#version 330 core

//Input variables
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 FragScreenPos;

//Output variables
out vec4 color;

//Structs
struct PointLight {    
    vec3 Position;
	vec3 Color;
	float Range;
	float Brightness;
};

//Uniforms
uniform int DSL = 0; //Double Side Lighting
uniform int DistanceLighting = 0;
uniform int SelfIlluminated = 0;

uniform int LightCount;
uniform PointLight PointLights[36];

uniform sampler2D MainTexture;
uniform sampler2D NormalTexture;

uniform int SourceFrames = 1;
uniform int CurrentFrame = 1;

uniform int OutlineMode = 0; //0 None, 1 Lit, 2 Non Lit
uniform float OutlineDivisor = 1;

uniform vec2 TextureSize;
uniform vec2 TextureTrans;

//Functions
vec4 CalculatePointLight(PointLight P, vec3 Normal, vec3 FragPos);
void Outline();

//Local Variables
float AV =  .05f;
vec4 AmbientColor = vec4(AV,AV,AV, 1);
vec4 ShadowColor = vec4(56f / 255f, 50f / 255f, 24f / 255f, 1.0f);
vec4 specColor = vec4(0,0,0,1);
vec4 preColor, imgColor;
vec2 curPixel, postTextCoord;

vec4 runTotal;

void main()
{
	
	vec3 normal = normalize(Normal);
	//specColor = texture(NormalTexture, curPixel);
		
	postTextCoord = TexCoord + TextureTrans;
		
	curPixel = vec2(
		(CurrentFrame / float(SourceFrames)) + (postTextCoord.x / SourceFrames), 
		(-postTextCoord.y));
	
	imgColor = texture(MainTexture, curPixel);
	
	preColor = imgColor;
	
	if (preColor.a < 1f){
		if (OutlineMode >= 1)
			Outline();
		else
			discard;
	}
	else {
		runTotal = ShadowColor * AmbientColor;
		
		//Non Self Illuminated Objects
		if (SelfIlluminated != 1){
			for (int i = 0; i < LightCount; i++){
				vec4 LightResult = CalculatePointLight(PointLights[i], normal, FragPos);
				runTotal += LightResult;
			}
			
			preColor *= runTotal;
			preColor.rgb = clamp(preColor.rgb, vec3(0,0,0), imgColor.rgb).rgb;
			
			for (int i = 0; i < LightCount; i++){
				preColor.rgb *= PointLights[i].Brightness;
			}
		}
	}
	
	color = preColor;
}

void Outline(){
	float neighborAlpha = 0;
	
	float xSize = 1 / (TextureSize.x * OutlineDivisor);
	float ySize = 1 / (TextureSize.y * OutlineDivisor);
	
	if (curPixel.x + xSize < 1)
		neighborAlpha += texture(MainTexture, curPixel + vec2(xSize, 0)).a;

	if (curPixel.x - xSize > 0)
		neighborAlpha += texture(MainTexture, curPixel - vec2(xSize, 0)).a;
	
	if (curPixel.y + ySize < 1)
		neighborAlpha += texture(MainTexture, curPixel + vec2(0, ySize)).a;
	
	if (curPixel.y - ySize > -1)
		neighborAlpha += texture(MainTexture, curPixel - vec2(0, ySize)).a;

	vec4 OutlineColor = 
	//vec4(0,0,0,1);
		vec4(1,TexCoord,1);
	
	if (neighborAlpha > 0){
		preColor = OutlineColor; //vec4(0,0,0,1);
	}
	else
		discard;
}

vec4 CalculatePointLight(PointLight P, vec3 _normal, vec3 FragPos){
	vec3 LightToFragVec = P.Position - FragPos;
	vec3 lightDir = normalize(LightToFragVec);
	
	float DistFromLight = length(LightToFragVec);
	
	float intensity = 0;
	if (DSL == 1)
		intensity = abs(dot(_normal, lightDir));
	else
		intensity = max(dot(_normal, lightDir), 0.0);
		
	if (DistanceLighting == 1)
		intensity = clamp(intensity, .35f,  1f);
	
	float Attenuation = clamp(P.Range / pow(DistFromLight, 3), 0, 1);
	
	vec4 Result = (vec4(P.Color, 1) * 
			intensity * Attenuation);
	
	return Result;
}

//preColor.gb *= TexCoord; //Adds pretty gradient
//preColor.rgb *= abs(Normal.xyz); //Illustrates face normals