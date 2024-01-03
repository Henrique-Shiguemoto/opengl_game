#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "types.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "stb_image.h"

class Model {
public:
	Model(const char *path);
	void Draw(Shader &shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};