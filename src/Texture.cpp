#include "Texture.h"

#include "stb_image.h"

Texture::Texture(const char* filepath, i32 filterMode, GLenum pixelFormat, i32 slot, b8 generateMipmap){
	this->slot = slot;
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	i32 imageWidth, imageHeight, imageChannelCount;
	u8* imageData = stbi_load(filepath, &imageWidth, &imageHeight, &imageChannelCount, 0);
	if (!imageData){
		std::cout << "Failed to load texture\n";
		Unbind();
		Delete();
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, pixelFormat, GL_UNSIGNED_BYTE, imageData);
	if(generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
}

void Texture::Activate(){
	glActiveTexture(this->slot);
}

void Texture::Bind(){
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete(){
	glDeleteTextures(1, &this->id);
}
