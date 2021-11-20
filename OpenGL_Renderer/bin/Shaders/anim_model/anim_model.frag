#version 330 core

//uniform sampler2D diffuseTex;
uniform vec4 albedo_color; 
uniform sampler2D albedoMap;


in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

out vec4 fragColour;

void main(void) {
    vec4 v4_albedo  = texture(albedoMap, fs_in.TexCoords) * albedo_color;
    vec3 albedo     = pow(v4_albedo.rgb, vec3(2.2));
    // if(value.a == 0.0) {
    //     discard;
    // }
    
    fragColour = vec4(albedo,1.0);
}