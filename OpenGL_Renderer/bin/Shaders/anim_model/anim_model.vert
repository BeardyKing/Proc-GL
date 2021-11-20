#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

in vec4 jointWeights ;
in ivec4 jointIndices ;
uniform mat4 joints [128];

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;
uniform vec2 textureScale;

out vec2 TexCoords;

void main(){
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = vec2(aTexCoords.x * textureScale.x, aTexCoords.y * textureScale.y);
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


// #version 330

// uniform mat4 projection;
// uniform mat4 view;
// uniform mat4 model;

// in vec3 position ;
// in vec2 texCoord ;


// out Vertex {
//     vec2 texCoord ;
// } OUT ;

// void main ( void ) {
//     vec4 localPos = vec4 (position , 1.0f);
//     vec4 skelPos = vec4 (0 ,0 ,0 ,0);

//     for (int i = 0; i < 4; ++i ) {
//     int jointIndex = jointIndices [ i ];
//         float jointWeight = jointWeights [ i ];
//         skelPos += joints [ jointIndex ] * localPos * jointWeight ;
//     }
    
//     mat4 mvp = projMatrix * viewMatrix * modelMatrix ;
//     gl_Position = mvp * vec4 ( skelPos . xyz , 1.0);
//     OUT . texCoord = texCoord ;
//  }
