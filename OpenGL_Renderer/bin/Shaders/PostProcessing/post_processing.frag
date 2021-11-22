#version 330 core
out vec4 FragColor;
  
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D rendertextureColor;
uniform sampler2D rendertextureDepth;

void main(){ 
    FragColor = texture(rendertextureColor, fs_in.TexCoords) * vec4(1,0,1,1);
    FragColor = vec4(vec3(texture(rendertextureDepth, fs_in.TexCoords).r), 1) ;//* vec4(1,0,1,1);
}