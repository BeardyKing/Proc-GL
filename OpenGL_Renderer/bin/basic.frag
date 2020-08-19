#version 330 core

uniform vec4 vertCol;

out vec4 frag_colour;

void main()
{
	frag_colour = vertCol; 
};