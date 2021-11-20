// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aTexCoords;

// in vec4 jointWeights;
// in ivec4 jointIndices;
// uniform mat4 joints [128];

// out VS_OUT {
//     vec3 FragPos;
//     vec3 Normal;
//     vec2 TexCoords;
//     vec4 FragPosLightSpace;
// } vs_out;

// uniform mat4 projection;
// uniform mat4 view;
// uniform mat4 model;

// uniform mat4 lightSpaceMatrix;
// uniform vec2 textureScale;

// out vec2 TexCoords;

// void main(){

//     vec4 localPos = vec4 (aPos , 1.0);
//     vec4 skelPos = vec4 (0 ,0 ,0 ,0);

//     for (int i = 0; i < 4; ++i ) {
//     int jointIndex = jointIndices [ i ];
//         float jointWeight = jointWeights [ i ];
//         skelPos += joints [ jointIndex ] * localPos * jointWeight ;
//     }
    
//     mat4 mvp = projection * view * model; //vec4(aPos, 1.0);
//     vs_out.TexCoords = vec2(aTexCoords.x * textureScale.x, aTexCoords.y * textureScale.y);
//     vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
//     // TODO DOES NOT WORK IF NO JOIN DATA IS LOADED
//     gl_Position = mvp * localPos * vec4 (skelPos.xyz , 1.0); 

// }


#version 330
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 5) in vec4 jointWeights ;
layout (location = 6) in ivec4 jointIndices ;

// layout (location = 0) in vec3 position ;
// layout (location = 2) in vec2 texCoord ;
// layout (location = 5) in vec4 jointWeights ;
// layout (location = 6)in ivec4 jointIndices ;


uniform mat4 joints [128];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out Vertex {
    vec2 texCoord ;
} OUT ;

void main ( void ) {
    vec4 localPos = vec4 (position , 1.0f);
    vec4 skelPos = vec4 (1 ,1 ,1 ,1);

    for (int i = 0; i < 4; ++i ) {
    int jointIndex = jointIndices [ i ];
        float jointWeight = jointWeights [ i ];
        skelPos += joints [ jointIndex ] * localPos * jointWeight ;
    }
    
    mat4 mvp = projection * view * model ;
    gl_Position = mvp * vec4(skelPos.xyz , 1.0); 
    OUT . texCoord = texCoord ;
 }
