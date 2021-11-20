#version 330 core

//uniform sampler2D diffuseTex;
uniform sampler2D albedoMap;


in Vertex {
    vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void) {
    vec4 value = texture(albedoMap , IN.texCoord ).rgba;

    if(value.a == 0.0) {
        discard;
    }
    
    fragColour = value;
}