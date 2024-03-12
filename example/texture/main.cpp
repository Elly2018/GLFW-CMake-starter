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

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
  if (w > h) {
    glViewport(0, (h - w) / 2, w, w);
  }
  else {
    glViewport((w - h) / 2, 0, h, h);
  }
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
    std::cout << "face count: " << face << std::endl;

    Shader ourShader("color_vs", "color_fs");
    unsigned int VBO, VAO, IND;
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
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    double time = glfwGetTime();
    mat4 idx, view, projection, mvp;
    glm_mat4_identity(idx);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    glm_mat4_identity(mvp);
    glm_translate(view, new float[3] { 0, 0, -5 });
    glm_perspective_default(640 / 480, projection);
    mat4* ms[3] = { &projection, &view, &idx };
    glm_mat4_mulN(ms, 3, mvp);
    glm_mat4_print(idx, stderr);
    glm_mat4_print(view, stderr);
    glm_mat4_print(projection, stderr);
    glm_mat4_print(mvp, stderr);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      std::cerr << "Error while creating mesh!" << std::endl;
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.4f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();

        double current = glfwGetTime();
        double delta = current - time;
        time = current;
        glm_rotate(idx, delta * 0.5f , new float[3] { 0.0f, 1.0f, 0.0f });
        mat4* ms[3] = { &projection, &view, &idx };
        glm_mat4_mulN(ms, 3, mvp);

        ourShader.setMat4("mvp", false, (float*)mvp);
        ourShader.setFloat3("ourColor", new float [3] { 1.0f, 0.5f, 0.5f });
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
        glDrawElements(GL_TRIANGLES, face * 3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
