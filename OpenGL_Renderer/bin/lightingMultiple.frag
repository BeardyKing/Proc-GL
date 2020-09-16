#version 330 core

struct Material{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;

	float cosInnerCone;
	float cosOuterCone;
	bool  on;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;
};


in vec2 TexCoord; 
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_colour;

uniform DirectionalLight	directionalLight;
uniform SpotLight			spotLight;
uniform PointLight			pointLight;

uniform Material material;


uniform vec3 viewPos;	// cam pos

vec3 calcSpotlight();
vec3 calcDirectionalLight();
vec3 calcPointLight();

void main(){ 
	//ambient
	vec3 ambient = spotLight.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord)); 
 	
	vec3 spotColor = vec3(0.0f);

	if(spotLight.on){
		spotColor = calcSpotlight();
	}

	vec3 directionalColor = vec3(0.0f);
	directionalColor = calcDirectionalLight();

	vec3 pointColor = vec3(0.0f);
	pointColor = calcPointLight();

	vec3 outCol = vec3(0.0f);
	outCol = spotColor + directionalColor + pointColor;

	frag_colour = vec4(ambient + outCol ,1.0f);
};

vec3 calcPointLight(){
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(pointLight.position - FragPos);
	float NDotL		= max(dot(normal, lightDir), 0.0f);
	vec3 temp = pointLight.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
	vec3 diffuse = temp;

	//specular (blit-phong)
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float NDotH = max(dot(normal,halfDir),0.0f);
	vec3 specular = pointLight.specular * material.specular * pow(NDotH, material.shininess);

	float d = length(pointLight.position - FragPos);
	float attenuation = 1.0f / (pointLight.constant + pointLight.linear * d + pointLight.exponent * (d * d));

	diffuse *= attenuation;
	specular *= attenuation;

	return (diffuse + specular);
}


vec3 calcDirectionalLight(){
	//diffuse
	vec3 normal		= normalize(Normal);
	vec3 lightDir	= normalize(-directionalLight.direction);
	float NDotL		= max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = directionalLight.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;;

	//specular (blit-phong)
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float NDotH = max(dot(normal,halfDir),0.0f);
	vec3 specular = directionalLight.specular * material.specular * pow(NDotH, material.shininess);
	
	return (diffuse + specular);
}

vec3 calcSpotlight(){
	//diffuse
	vec3 lightDir	= normalize(spotLight.position - FragPos);
	vec3 spotDir	= normalize(spotLight.direction);

	float cosDir = dot(-lightDir, spotDir);
	float spotIntensity = smoothstep(spotLight.cosOuterCone, spotLight.cosInnerCone, cosDir);

	vec3 normal		= normalize(Normal);
	float NDotL		= max(dot(normal, lightDir), 0.0f);
	vec3 diffuse	= spotLight.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;;

	//specular (blinn-phong)
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal,halfDir),0.0f);
	vec3 specular = spotLight.specular * material.specular * pow(NDotH, material.shininess);

	float d = length(spotLight.position - FragPos);
	float attenuation = 1.0f / (spotLight.constant + spotLight.linear * d + spotLight.exponent * (d * d));

	diffuse *= attenuation * spotIntensity;
	specular *= attenuation * spotIntensity;

	return (diffuse + specular);
}

