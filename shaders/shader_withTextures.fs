#version 330 core

out vec4 outputColor;

in vec2 textureCoordFromVertexShader;
in vec3 outNormals;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D cursorTexture;
uniform vec3 cameraPos;

void main(){
	float ambientLighting = 0.1f;
	vec3 ambient = ambientLighting * lightColor;

	vec3 norm = normalize(outNormals);
	vec3 lightDir = normalize(lightPos - fragPos);

	float specularStrength = 0.5;
	vec3 cameraViewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float diffuseCoefficient = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseCoefficient * lightColor;

	float spec = pow(max(dot(cameraViewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec4 objectColor = texture(cursorTexture, textureCoordFromVertexShader);
	outputColor = vec4(ambient + diffuse + specular, 1.0f) * objectColor;
}