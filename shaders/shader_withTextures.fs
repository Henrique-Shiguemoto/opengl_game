#version 330 core

out vec4 outputColor;

in vec2 textureCoordFromVertexShader;
in vec3 outNormals;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D cursorTexture;

void main(){
	float ambientLighting = 0.1f;
	vec3 ambient = ambientLighting * lightColor;

	vec3 norm = normalize(outNormals);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuseCoefficient = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseCoefficient * lightColor;
	outputColor = vec4(ambient + diffuse, 1.0f) * texture(cursorTexture, textureCoordFromVertexShader);
}