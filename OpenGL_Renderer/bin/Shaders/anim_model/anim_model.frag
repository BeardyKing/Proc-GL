#version 330 core

//uniform sampler2D diffuseTex;
uniform vec4 albedo_color; 
uniform sampler2D albedoMap;


in Vertex {
    vec2 texCoord ;
} IN ;

out vec4 fragColour;

void main(void) {
    vec4 v4_albedo  = texture(albedoMap, IN.texCoord) * albedo_color;
    vec3 albedo     = pow(v4_albedo.rgb, vec3(2.2));
    // if(value.a == 0.0) {
    //     discard;
    // }
    
    fragColour = vec4(albedo,1.0);
}