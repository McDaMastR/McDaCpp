#pragma once

#include <string>
#include <unordered_map>

#include "../vendor/glm/glm.hpp"

class Shader
{
private:
    std::string path;
    unsigned int rendererID;
    mutable std::unordered_map<std::string, int> uniform_location_cache;

public:
    Shader(const std::string &path);
    ~Shader();

    void   bind() const;
    void unBind() const;

    void setUniformi(const std::string &name, const int v0);
    void setUniformf(const std::string &name, const float v0, const float v1, const float v2, const float v3);

    void setUniformMatf(const std::string &name, const glm::mat4 &matrix);

private:
    unsigned int getUniformLocation(const std::string &name) const;
    std::pair<std::string, std::string> parseShader(const std::string &path);
    unsigned int createShader(const std::string &vertex_shader, const std::string &fragment_shader);
    unsigned int compileShader(const unsigned int type, const std::string &source);
};
