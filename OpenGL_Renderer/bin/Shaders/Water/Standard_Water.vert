#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    vec4 ClipSpace;

    mat4 view;
    mat4 projection;
    mat4 invView;
    mat4 invProjection;

} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform vec2 textureScale;

void main()
{
    vs_out.view = view;
    vs_out.projection = projection;
    vs_out.invView = transpose(inverse(view));
    vs_out.invProjection = transpose(inverse(projection));

    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = vec2(aTexCoords.x * textureScale.x, aTexCoords.y * textureScale.y);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vs_out.ClipSpace = projection * view * model * vec4(aPos.xyz, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
}