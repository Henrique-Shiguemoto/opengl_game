#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

out vec2 textureCoord;
out vec3 outNormals;
out vec3 fragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0f);
	textureCoord = texCoord;
	outNormals = normals;
	fragPos = vec3(modelMatrix * vec4(vertexPos, 1.0f));
}
