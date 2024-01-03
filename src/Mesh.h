#pragma once

#include "types.h"
#include <vector>
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
