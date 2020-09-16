#version 330 core

struct Material{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoord; 
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform DirectionalLight light;
uniform Material material;


uniform vec3 viewPos;	// cam pos

void main(){ 
	//ambient
	vec3 ambient = light.ambient * material.ambient; 
 	//diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(-light.direction);
	float NDotL		= max(dot(normal, lightDir), 0.0f);
	vec3 temp = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
	temp = temp + vec3(texture(material.diffuseMap, TexCoord) / 4 ); // make texture visible when no light
	vec3 diffuse = temp;

	//specular (blit-phong)
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float NDotH = max(dot(normal,halfDir),0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	frag_colour = vec4(ambient + diffuse + specular,1.0f);
};
