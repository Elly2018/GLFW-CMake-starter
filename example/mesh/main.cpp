#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cglm/cglm.h>
#include <string>
#include <iostream>
#include "shader.h"

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "GLFW CMake starter", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

    const aiScene* scene = aiImportFile("monkey.obj", aiProcessPreset_TargetRealtime_Fast);
    aiMesh* mesh = scene->mMeshes[0];

    unsigned int face = mesh->mNumFaces;
    unsigned int d = 0;
    unsigned int* index_array = new unsigned int[face * 3 * sizeof(unsigned int)];
    for (int i = 0; i < face; i++) {
      const struct aiFace* vf = &(mesh->mFaces[i]);
      index_array[d++] = (vf->mIndices[0]);
      index_array[d++] = (vf->mIndices[1]);
      index_array[d++] = (vf->mIndices[2]);
    }

    Shader ourShader("color_vs", "color_fs");
    unsigned int VBO, VAO, IND;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &IND);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * face, index_array, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glDisable(GL_CULL_FACE);
    
    mat4 idx, view, projection, mvp;
    glm_mat4_identity(idx);
    glm_mat4_identity(view);
    glm_mat4_identity(mvp);
    glm_translate(view, new float[3] { 0, 0, 5 });
    glm_perspective_default(640 / 480, projection);
    glm_mat4_mul(idx, view, mvp);
    glm_mat4_mul(mvp, projection, mvp);
    glUniform4fv(0, 1, false, mvp[0][0]);
    glUniform3f(1, 1.0f, 0.5f, 0.5f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.4f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
