#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader
{
private:
    std::string m_vertexFilePath;
    std::string m_fragmentFilePath;
    uint32_t m_rendererID;
    mutable std::unordered_map<std::string, int32_t> m_uniformLocationCache;

public:
    Shader(const std::string &vertex_path, const std::string &fragment_path);
    ~Shader();

    void   bind() const;
    void unBind() const;

    void setUniformVeci(const std::string &name, const int32_t v0) const;
    void setUniformVecf(const std::string &name, const glm::vec4 &values) const;

    void setUniformMatf(const std::string &name, const glm::mat4 &matrix) const;

private:
    unsigned int getUniformLocation(const std::string &name) const;
    std::pair<std::string, std::string> parseShader(const std::string &vertex_shader, const std::string &fragment_shader) const;
    void createShader(const std::string &vertex_shader, const std::string &fragment_shader);
    unsigned int compileShader(const uint32_t type, const std::string &source) const;
};
