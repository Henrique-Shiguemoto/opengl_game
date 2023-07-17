#include "VertexArray.h"

VertexArray::VertexArray(){
	glGenVertexArrays(1, &id);
}

void VertexArray::DefineVBOLayout(VertexBuffer* vbo, u32 layoutIndex, i32 sizeInComponents, u32 strideInBytes, u32 offsetInComponents){
	vbo->Bind();
	glVertexAttribPointer(layoutIndex, sizeInComponents, GL_FLOAT, GL_FALSE, strideInBytes, (void*)(offsetInComponents * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(layoutIndex);
	vbo->Unbind();
}

void VertexArray::Bind(){
	glBindVertexArray(id);
}

void VertexArray::Unbind(){
	glBindVertexArray(0);
}

void VertexArray::Delete(){
	glDeleteVertexArrays(1, &id);
}
