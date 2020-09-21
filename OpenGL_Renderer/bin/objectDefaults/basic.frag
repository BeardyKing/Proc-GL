#version 330 core

uniform vec3 lightCol;

out vec4 frag_colour;


void main(){
	frag_colour = vec4(lightCol, 1.0f);
};