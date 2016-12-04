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

uniform vec2 SourceRect = vec2(0,0);
uniform int SourceFrame = 1;

void main()
{
	
    gl_Position = projection * view * model * vec4(position, 1.0f);

	FragPos = vec3(model * vec4(position, 1.0f));
	
	Normal = (vec4(normal, 1.0f) * normalMatrix).xyz;
	
    TexCoord = texCoord;
	
	
}