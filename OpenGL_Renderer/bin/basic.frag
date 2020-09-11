#version 330 core

uniform vec4 vertCol;

in vec2 TexCoord; 
out vec4 frag_colour;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main(){
	frag_colour = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), 0.5); 
};