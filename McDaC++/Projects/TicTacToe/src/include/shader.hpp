#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

enum class ShaderType
{
    vertex, fragment
};

class Shader
{
private:
    GLuint m_programID;
    std::unordered_map<ShaderType, std::string> m_shaders;
    mutable std::unordered_map<std::string, std::int32_t> m_uniformLocationCache;

public:
    Shader(const std::string &file_path);
    ~Shader();

    void   bind() const;
    void unBind() const;

    void setUniformVeci(const std::string &name, const std::int32_t v0) const;
    void setUniformVecf(const std::string &name, const glm::vec4 &values) const;

    void setUniformMatf(const std::string &name, const glm::mat4 &matrix) const;

private:
    std::uint32_t getUniformLocation(const std::string &name) const;
    std::string parseShader(const std::string &file_path) const;
    void createProgram(const std::string &vertex_shader, const std::string &fragment_shader);
    std::uint32_t compileShader(const std::uint32_t type, const std::string &source) const;
    void checkError(const std::uint32_t flag, const bool is_program, const std::uint32_t shader_id = 0, const std::uint32_t shader_type = 0) const;
};
