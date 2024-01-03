#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 dimension, glm::vec3 color, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular){
	this->pos = pos;
	this->dimension = dimension;
	this->color = color;
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->specular = specular;

	#define LBN_LIGHT this->pos.x - 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define LTN_LIGHT this->pos.x - 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define LTF_LIGHT this->pos.x - 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define LBF_LIGHT this->pos.x - 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define RBN_LIGHT this->pos.x + 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define RTN_LIGHT this->pos.x + 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define RTF_LIGHT this->pos.x + 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define RBF_LIGHT this->pos.x + 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z

	f32 lightVertices[] = {
		//position + color
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBN_LIGHT, 1.0f, 1.0f, 1.0f,
		LBF_LIGHT, 1.0f, 1.0f, 1.0f,
		RBF_LIGHT, 1.0f, 1.0f, 1.0f,
		LTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		LTF_LIGHT, 1.0f, 1.0f, 1.0f,
		RTN_LIGHT, 1.0f, 1.0f, 1.0f,
		RTF_LIGHT, 1.0f, 1.0f, 1.0f
	};

	u32 lightIndices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32, 
		33, 34, 35
	};

	this->vaoLight = new VertexArray();
	this->vaoLight->Bind();
	this->vboLight = new VertexBuffer(lightVertices, sizeof(lightVertices));
	this->iboLight = new IndexBuffer(lightIndices, sizeof(lightIndices));
	this->vaoLight->DefineVBOLayout(vboLight, 0, 3, 24, 0);
	this->vaoLight->DefineVBOLayout(vboLight, 1, 3, 24, 3);
}