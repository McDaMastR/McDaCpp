#pragma once

#include <string>
#include <unordered_map>

#include "../vendor/glm/glm.hpp"

class Shader
{
private:
    std::string m_filePath;
    unsigned int m_rendererID;
    mutable std::unordered_map<std::string, int> m_uniformLocationCache;

public:
    Shader(const std::string &file_path);
    ~Shader();

    void   bind() const;
    void unBind() const;

    void setUniformVeci(const std::string &name, const int v0);
    void setUniformVecf(const std::string &name, const glm::vec4 &values);

    void setUniformMatf(const std::string &name, const glm::mat4 &matrix);

private:
    unsigned int getUniformLocation(const std::string &name) const;
    std::pair<std::string, std::string> parseShader(const std::string &path);
    unsigned int createShader(const std::string &vertex_shader, const std::string &fragment_shader);
    unsigned int compileShader(const unsigned int type, const std::string &source);
};
