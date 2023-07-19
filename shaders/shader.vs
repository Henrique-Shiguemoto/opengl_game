#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;
out vec3 colorFromVertexShader;

uniform vec3 playerPos;
uniform mat4 transform;
// uniform mat4 view;
// uniform mat4 perspective;

void main(){
	gl_Position = transform * vec4(vertexPos, 1.0f);
	colorFromVertexShader = vertexColor;
}