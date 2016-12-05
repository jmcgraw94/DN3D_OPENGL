#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 normalMatrix;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float Time;
uniform vec3 WorldPos;
uniform float WaveFactor;

void main()
{	
	vec4 prePos = vec4(position, 1.0f);
	mat4 MVP = projection * view * model;
	
	float yWave = (sin((Time * 2) + WorldPos.x + position.x) / 3.0f) * WaveFactor;
	float xWave = (cos((Time * 2) + WorldPos.x + position.x) / 3.0f) * WaveFactor;
	
    gl_Position = MVP * (prePos + vec4(xWave, yWave,xWave,0));

	FragPos = vec3(model * vec4(position, 1.0f));
	
	Normal = (vec4(normal, 1.0f) * normalMatrix).xyz;
	
    TexCoord = texCoord;
	
	
}