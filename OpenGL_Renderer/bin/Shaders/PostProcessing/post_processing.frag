#version 330 core
out vec4 FragColor;
  
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D screenTexture;

void main(){ 
    FragColor = texture(screenTexture, fs_in.TexCoords) * vec4(1,0,1,1);
}