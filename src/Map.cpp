#include "Map.h"

Map::Map(glm::vec3 pos, glm::vec3 dimension){
	this->pos = pos;
	this->dimension = dimension;

	#define L_NORMAL -1.0f,  0.0f,  0.0f
	#define R_NORMAL  1.0f,  0.0f,  0.0f
	#define N_NORMAL  0.0f,  0.0f,  1.0f
	#define F_NORMAL  0.0f,  0.0f, -1.0f
	#define T_NORMAL  0.0f,  1.0f,  0.0f
	#define B_NORMAL  0.0f, -1.0f,  0.0f

	#define LBN_MAP this->pos.x - 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define LTN_MAP this->pos.x - 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define LTF_MAP this->pos.x - 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define LBF_MAP this->pos.x - 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define RBN_MAP this->pos.x + 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define RTN_MAP this->pos.x + 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z + 0.5f*this->dimension.z
	#define RTF_MAP this->pos.x + 0.5f*this->dimension.x, this->pos.y + 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z
	#define RBF_MAP this->pos.x + 0.5f*this->dimension.x, this->pos.y - 0.5f*this->dimension.y, this->pos.z - 0.5f*this->dimension.z

	f32 mapVertices[] = {
		//position + texture coordinates + normals
		LBN_MAP, 0.0f, 0.0f, L_NORMAL,
		LTN_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, L_NORMAL,
		LBN_MAP, 0.0f, 0.0f, L_NORMAL,
		LBF_MAP, 0.0f, 0.0f, L_NORMAL,
		LTF_MAP, 0.0f, 0.0f, F_NORMAL,
		LBF_MAP, 0.0f, 0.0f, F_NORMAL,
		RBF_MAP, 0.0f, 0.0f, F_NORMAL,
		LTF_MAP, 0.0f, 0.0f, F_NORMAL,
		RBF_MAP, 0.0f, 0.0f, F_NORMAL,
		RTF_MAP, 0.0f, 0.0f, F_NORMAL,
		RTF_MAP, 0.0f, 0.0f, R_NORMAL,
		RBF_MAP, 0.0f, 0.0f, R_NORMAL,
		RTN_MAP, 0.0f, 0.0f, R_NORMAL,
		RTN_MAP, 0.0f, 0.0f, R_NORMAL,
		RBF_MAP, 0.0f, 0.0f, R_NORMAL,
		RBN_MAP, 0.0f, 0.0f, R_NORMAL,
		RBN_MAP, 0.0f, 0.0f, N_NORMAL,
		RTN_MAP, 0.0f, 0.0f, N_NORMAL,
		LTN_MAP, 0.0f, 0.0f, N_NORMAL,
		LTN_MAP, 0.0f, 0.0f, N_NORMAL,
		RBN_MAP, 0.0f, 0.0f, N_NORMAL,
		LBN_MAP, 0.0f, 0.0f, N_NORMAL,
		RBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBF_MAP, 0.0f, 0.0f, B_NORMAL,
		RBN_MAP, 0.0f, 0.0f, B_NORMAL,
		LBF_MAP, 0.0f, 0.0f, B_NORMAL,
		RBF_MAP, 0.0f, 0.0f, B_NORMAL,
		LTN_MAP, 0.0f, 0.0f, T_NORMAL,
		LTF_MAP, 0.0f, 1.0f, T_NORMAL,
		RTN_MAP, 1.0f, 0.0f, T_NORMAL,
		LTF_MAP, 0.0f, 1.0f, T_NORMAL,
		RTN_MAP, 1.0f, 0.0f, T_NORMAL,
		RTF_MAP, 1.0f, 1.0f, T_NORMAL
	};

	u32 mapIndices[] = {
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

	this->point_from_map = this->pos + glm::vec3(0.0f, 0.5f * this->dimension.y, 0.0f); // this is only useful if the map is completely flat and is aligned with the coordinate system.

	this->vaoMap = new VertexArray();
	this->vaoMap->Bind();
	this->vboMap = new VertexBuffer(mapVertices, sizeof(mapVertices));
	this->iboMap = new IndexBuffer(mapIndices, sizeof(mapIndices));
	this->vaoMap->DefineVBOLayout(vboMap, 0, 3, 32, 0);
	this->vaoMap->DefineVBOLayout(vboMap, 1, 2, 32, 3);
	this->vaoMap->DefineVBOLayout(vboMap, 2, 3, 32, 5);
}