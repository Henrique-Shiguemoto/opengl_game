#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(u32* data, u32 sizeInBytes){
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);
}

void IndexBuffer::SetupIndexBuffer(u32* data, u32 sizeInBytes){
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);	
}

void IndexBuffer::Bind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Delete(){
	glDeleteBuffers(1, &id);
}
