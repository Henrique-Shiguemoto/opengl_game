#version 330 core

out vec4 outputColor;

in vec3 outNormals;
in vec2 textureCoord;
in vec3 fragPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

uniform vec3 lightPos;
uniform vec3 cameraPos;

void main(){
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoord));

	//diffuse
	vec3 norm = normalize(outNormals);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, textureCoord)));

	//specular
	vec3 cameraViewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(cameraViewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, textureCoord)));

	vec3 colorResult = ambient + diffuse + specular;
	outputColor = vec4(colorResult, 1.0f);
}
