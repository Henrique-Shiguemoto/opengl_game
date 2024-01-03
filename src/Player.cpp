#include "Player.h"

Player::Player(glm::vec3 pos, glm::vec3 dimension, f32 maxSpeed){
	this->pos = pos;
	this->dimension = dimension;
	this->maxSpeed = maxSpeed;
	this->locationPlayerHasToGo = this->pos;

	#define L_NORMAL -1.0f,  0.0f,  0.0f
	#define R_NORMAL  1.0f,  0.0f,  0.0f
	#define N_NORMAL  0.0f,  0.0f,  1.0f
	#define F_NORMAL  0.0f,  0.0f, -1.0f
	#define T_NORMAL  0.0f,  1.0f,  0.0f
	#define B_NORMAL  0.0f, -1.0f,  0.0f

	#define LBN_PLAYER -0.5f * this->dimension.x, -0.5f * this->dimension.y, +0.5f * this->dimension.z
	#define LTN_PLAYER -0.5f * this->dimension.x, +0.5f * this->dimension.y, +0.5f * this->dimension.z
	#define LTF_PLAYER -0.5f * this->dimension.x, +0.5f * this->dimension.y, -0.5f * this->dimension.z
	#define LBF_PLAYER -0.5f * this->dimension.x, -0.5f * this->dimension.y, -0.5f * this->dimension.z
	#define RBN_PLAYER +0.5f * this->dimension.x, -0.5f * this->dimension.y, +0.5f * this->dimension.z
	#define RTN_PLAYER +0.5f * this->dimension.x, +0.5f * this->dimension.y, +0.5f * this->dimension.z
	#define RTF_PLAYER +0.5f * this->dimension.x, +0.5f * this->dimension.y, -0.5f * this->dimension.z
	#define RBF_PLAYER +0.5f * this->dimension.x, -0.5f * this->dimension.y, -0.5f * this->dimension.z

	f32 playerVertices[] = {
		//position + uv + normals
		LBN_PLAYER, 0.0f, 0.0f, L_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, L_NORMAL,
		LTF_PLAYER, 1.0f, 1.0f, L_NORMAL,
		LTF_PLAYER, 1.0f, 1.0f, L_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, L_NORMAL,
		LBF_PLAYER, 1.0f, 0.0f, L_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, F_NORMAL,
		LBF_PLAYER, 0.0f, 0.0f, F_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, F_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, F_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, F_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, F_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, R_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, R_NORMAL,
		RTN_PLAYER, 0.0f, 1.0f, R_NORMAL,
		RTN_PLAYER, 0.0f, 1.0f, R_NORMAL,
		RBF_PLAYER, 1.0f, 0.0f, R_NORMAL,
		RBN_PLAYER, 0.0f, 0.0f, R_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, N_NORMAL,
		RTN_PLAYER, 1.0f, 1.0f, N_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, N_NORMAL,
		LTN_PLAYER, 0.0f, 1.0f, N_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, N_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, N_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, B_NORMAL,
		LBN_PLAYER, 0.0f, 0.0f, B_NORMAL,
		LBF_PLAYER, 0.0f, 1.0f, B_NORMAL,
		RBN_PLAYER, 1.0f, 0.0f, B_NORMAL,
		LBF_PLAYER, 0.0f, 1.0f, B_NORMAL,
		RBF_PLAYER, 1.0f, 1.0f, B_NORMAL,
		LTN_PLAYER, 0.0f, 0.0f, T_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, T_NORMAL,
		RTN_PLAYER, 1.0f, 0.0f, T_NORMAL,
		LTF_PLAYER, 0.0f, 1.0f, T_NORMAL,
		RTN_PLAYER, 1.0f, 0.0f, T_NORMAL,
		RTF_PLAYER, 1.0f, 1.0f, T_NORMAL
	};

	u32 playerIndices[] = {
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

	this->vaoPlayer = new VertexArray();
	this->vaoPlayer->Bind();
	this->vboPlayer = new VertexBuffer(playerVertices, sizeof(playerVertices));
	this->iboPlayer = new IndexBuffer(playerIndices, sizeof(playerIndices));
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 0, 3, 32, 0);
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 1, 2, 32, 3);
	this->vaoPlayer->DefineVBOLayout(vboPlayer, 2, 3, 32, 5);
}