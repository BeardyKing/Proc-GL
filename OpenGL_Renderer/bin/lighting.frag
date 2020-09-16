#version 330 core

uniform vec4 vertCol;

in vec2 TexCoord; 
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

uniform vec3 lightCol;	// light colour
uniform vec3 lightPos;	// light source pos
uniform vec3 viewPos;	// cam pos

void main(){
	//ambient
	float ambientFactor = 0.1f;
	vec3 ambient = lightCol * ambientFactor; 
	
	//diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(lightPos - FragPos);
	float NDotL		= max(dot(normal, lightDir), 0.0f);
	vec3 diffuse	= lightCol * NDotL;

	//specular (blit-phong)
	float specularFactor = 0.8f;
	float shininess = 290.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float NDotH = max(dot(normal,halfDir),0.0f);
	vec3 specular = lightCol * specularFactor * pow(NDotH, shininess);

	vec4 texel = texture(myTexture1, TexCoord); 
	frag_colour = vec4(ambient + diffuse + specular,1.0f) * texel;
};
