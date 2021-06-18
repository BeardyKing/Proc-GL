//PBR.VERT
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

out vec4 FragPosLightSpace;
uniform mat4 lightSpaceMatrix;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    
    Normal = transpose(inverse(mat3(model))) * aNormal;
    TexCoords = aTexCoords;
    WorldPos = vec3(model * vec4(aPos, 1.0));
    FragPosLightSpace = lightSpaceMatrix * vec4(WorldPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}