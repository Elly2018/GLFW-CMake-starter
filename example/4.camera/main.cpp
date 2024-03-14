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
#include "camera.h"
#include "Mesh.h"

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

double __time = 0.0;
double delta() 
{
  double current = glfwGetTime();
  double a = current - __time;
  __time = current;
  return a;
}

int main(void)
{
    GLFWwindow* window;
    double movement = 1.0f;

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

    Mesh* mesh = Mesh::LoadFromFile("assets/mesh/monkey.obj");
    Camera camera(new float [3] {0.0f, -1.0f, -2.0f}, new float [3] {0.0f, 0.0f, 0.0f});
    Shader ourShader("assets/shader/color.vs", "assets/shader/color.fs");
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    __time = glfwGetTime();
    mat4 idx, view, projection, mrot, mvp;
    glm_mat4_identity(idx);
    glm_mat4_identity(mrot);
    glm_mat4_identity(mvp);
    camera.getView(&view);
    camera.getProjection(&projection);
    glm_translate(view, new float[3] { 0, 0, -5 });
    glm_perspective_default(640 / 480, projection);
    mat4* ms[3] = { &projection, &view, &idx };
    glm_mat4_mulN(ms, 3, mvp);

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

        double _delta = delta();
        //glm_rotate(idx, _delta * 0.5f , new float[3] { 0.0f, 1.0f, 0.0f }); // object self rotation

        vec3 move { 0, 0, 0 };
        vec3 rot{ 0, 0, 0 };
        if (glfwGetKey(window, GLFW_KEY_W))
        {
          move[2] += _delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
          move[0] += _delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
          move[0] += -_delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
          move[2] += -_delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
          move[1] += _delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE))
        {
          move[1] += -_delta * movement;
        }
        if (glfwGetKey(window, GLFW_KEY_Q))
        {
          rot[1] += -_delta * movement * 50;
        }
        if (glfwGetKey(window, GLFW_KEY_E))
        {
          rot[1] += _delta * movement * 50;
        }
        
        ourShader.setMat4("mvp", false, (float*)mvp);
        ourShader.setFloat3("ourColor", new float [3] { 1.0f, 0.5f, 0.5f });
        camera.move(move, true);
        //camera.rot(rot);

        camera.getView(&view);
        camera.getProjection(&projection);
        glm_rotate(mrot, glm_rad(rot[0]), new float[3] { 1, 0, 0 });
        glm_rotate(mrot, glm_rad(rot[1]), new float[3] { 0, 1, 0 });
        glm_rotate(mrot, glm_rad(rot[2]), new float[3] { 0, 0, 1 });
        mat4* ms[4] = { &projection, &mrot, &view, &idx};
        glm_mat4_mulN(ms, 4, mvp);
        mesh->bindVAO();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
