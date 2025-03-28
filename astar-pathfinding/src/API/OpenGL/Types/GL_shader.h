#pragma once

#include <string>
#include <unordered_map>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {

public:
    int _ID = -1;
    std::unordered_map<std::string, int> _uniformsLocations;

    void Load(std::string vertexPath, std::string fragmentPath);
    //void Load(std::string vertexPath, std::string fragmentPath, std::string geomPath);
    void Use();
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetMat4(const std::string& name, glm::mat4 value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec2(const std::string& name, const glm::vec2& value);
};
