#version 330 core

uniform vec4 vertCol;

in vec2 TexCoord; 
out vec4 frag_colour;

uniform sampler2D myTexture;

void main()
{
	frag_colour = texture(myTexture, TexCoord); 
};