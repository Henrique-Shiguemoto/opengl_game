#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 outColor;

void main(){
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0f);
	outColor = color;
}