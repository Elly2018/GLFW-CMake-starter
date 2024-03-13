#pragma once
#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cglm/cglm.h>
#include <string>
#include <iostream>

class Mesh 
{
public:
	void getModel(mat4* v) { glm_mat4_copy(model, *v); }

  void bindVAO() 
  {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
    glDrawElements(GL_TRIANGLES, face * 3, GL_UNSIGNED_INT, 0);
  }

	static Mesh* LoadFromFile(std::string file) 
	{
    return new Mesh(file);
	}

private:
  Mesh(std::string file)
  {
    const aiScene* scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_Fast);
    aiMesh* mesh = scene->mMeshes[0];

    face = mesh->mNumFaces;
    unsigned int d = 0;
    unsigned int* index_array = new unsigned int[face * 3 * sizeof(unsigned int)];
    for (int i = 0; i < face; i++) {
      const struct aiFace* vf = &(mesh->mFaces[i]);
      index_array[d++] = (vf->mIndices[0]);
      index_array[d++] = (vf->mIndices[1]);
      index_array[d++] = (vf->mIndices[2]);
    }
    std::cout << "face count: " << face << std::endl;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D) * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    std::cout << "vertices count: " << mesh->mNumVertices << std::endl;

    glGenBuffers(1, &IND);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face * 3 * sizeof(unsigned int), index_array, GL_STATIC_DRAW);
  }
  ~Mesh() 
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IND);
  }

	mat4 model;
  unsigned int face, VBO, VAO, IND;
};