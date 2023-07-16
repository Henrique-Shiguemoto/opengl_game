#version 330 core

out vec4 outputColor;
in vec3 colorFromVertexShader;

void main(){
	outputColor = vec4(colorFromVertexShader, 1.0f);
}