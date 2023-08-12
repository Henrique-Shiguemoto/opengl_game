#version 330 core

out vec4 outputColor;
in vec2 textureCoordFromVertexShader;

uniform sampler2D cursorTexture;

void main(){
	outputColor = texture(cursorTexture, textureCoordFromVertexShader);
}