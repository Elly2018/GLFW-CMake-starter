#pragma once
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <cglm/cglm.h>
#include "shader.h"

class Material 
{
public:
	Material(Shader* _shader) {
		shader = _shader;
	}
  ~Material()
  {
    delete boolMap;
    delete intMap;
    delete floatMap;
    delete vec2Map;
    delete vec3Map;
    delete vec4Map;
    delete mat4Map;
  }
  void useMaterial() 
  {
    shader->use();
    for (auto v : *boolMap) 
    {
      glUniform1i(glGetUniformLocation(shader->ID, v.first.c_str()), v.second);
    }
    for (auto v : *intMap)
    {
      glUniform1i(glGetUniformLocation(shader->ID, v.first.c_str()), v.second);
    }
    for (auto v : *floatMap)
    {
      glUniform1f(glGetUniformLocation(shader->ID, v.first.c_str()), v.second);
    }
    for (auto v : *vec2Map)
    {
      glUniform2f(glGetUniformLocation(shader->ID, v.first.c_str()), v.second[0], v.second[1]);
    }
    for (auto v : *vec3Map)
    {
      glUniform3f(glGetUniformLocation(shader->ID, v.first.c_str()), v.second[0], v.second[1], v.second[2]);
    }
    for (auto v : *vec4Map)
    {
      glUniform4f(glGetUniformLocation(shader->ID, v.first.c_str()), v.second[0], v.second[1], v.second[2], v.second[3]);
    }
    for (auto v : *mat4Map)
    {
      glUniformMatrix4fv(glGetUniformLocation(shader->ID, v.first.c_str()), 1, false, (float* )v.second);
    }
  }
  void setBool(const std::string& name, const bool value) const
  {
    boolMap->insert(std::pair<const std::string, const bool>(name, value));
  }
  void setInt(const std::string& name, const int value) const
  {
    intMap->insert(std::pair<const std::string, const int>(name, value));
  }
  void setFloat(const std::string& name, const float value) const
  {
    floatMap->insert(std::pair<const std::string, const float>(name, value));
  }
  void setFloat2(const std::string& name, const vec2 value) const
  {
    vec2Map->insert(std::pair<const std::string, const float*>(name, value));
  }
  void setFloat3(const std::string& name, const vec3 value) const
  {
    vec3Map->insert(std::pair<const std::string, const float*>(name, value));
  }
  void setFloat4(const std::string& name, const vec4 value) const
  {
    vec4Map->insert(std::pair<const std::string, const float*>(name, value));
  }
  void setMat4(const std::string& name, bool transpose, mat4 value) const
  {
    glm_mat4_transpose(value);
    mat4Map->insert(std::pair<const std::string, const vec4*>(name, value));
  }

private:
  std::map<const std::string, const bool>* boolMap = new std::map<const std::string, const bool>();
  std::map<const std::string, const int>* intMap = new std::map<const std::string, const int>();
  std::map<const std::string, const float>* floatMap = new std::map<const std::string, const float>();
  std::map<const std::string, const float*>* vec2Map = new std::map<const std::string, const float*>();
  std::map<const std::string, const float*>* vec3Map = new std::map<const std::string, const float*>();
  std::map<const std::string, const float*>* vec4Map = new std::map<const std::string, const float*>();
  std::map<const std::string, const vec4*>* mat4Map = new std::map<const std::string, const vec4*>();
	Shader* shader;
};