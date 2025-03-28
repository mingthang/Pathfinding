#include "GL_shader.h"
#include <glad/glad.h>
#include <iostream>
#include <Util/Util.h>

int checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- \n";
        }
    }
    return success;
}

void Shader::Load(std::string vertexPath, std::string fragmentPath) {

    std::string vertexSource = Util::ReadTextFromFile("res/shaders/OpenGL/" + vertexPath);
    std::string fragmentSource = Util::ReadTextFromFile("res/shaders/OpenGL/" + fragmentPath);

    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    int tempID = glCreateProgram();
    glAttachShader(tempID, vertex);
    glAttachShader(tempID, fragment);
    glLinkProgram(tempID);

    if (checkCompileErrors(tempID, "PROGRAM")) {
        if (_ID != -1) {
            glDeleteProgram(_ID);
        }
        _ID = tempID;
        _uniformsLocations.clear();
        //std::cout << "shader SUCCESFULLY compiled " << vertexPath << " " << fragmentPath << "\n";
    }
    else {
        std::cout << "shader failed to compile " << vertexPath << " " << fragmentPath << "\n";
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(_ID);
}

void Shader::SetBool(const std::string& name, bool value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniform1i(_uniformsLocations[name], (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniform1i(_uniformsLocations[name], value);
}

void Shader::SetFloat(const std::string& name, float value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniform1f(_uniformsLocations[name], value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniformMatrix4fv(_uniformsLocations[name], 1, GL_FALSE, &value[0][0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniform3fv(_uniformsLocations[name], 1, &value[0]);
}
void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    if (_uniformsLocations.find(name) == _uniformsLocations.end())
        _uniformsLocations[name] = glGetUniformLocation(_ID, name.c_str());
    glUniform2fv(_uniformsLocations[name], 1, &value[0]);
}