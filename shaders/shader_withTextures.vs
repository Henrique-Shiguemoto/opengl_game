#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 texCoord;

out vec2 textureCoordFromVertexShader;

uniform mat4 cursorOffsetMatrix;

void main(){
	gl_Position = cursorOffsetMatrix * vec4(vertexPos, 1.0f);
	textureCoordFromVertexShader = texCoord;
}
