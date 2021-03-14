#include "include/texture.hpp"
#include "vendor/stb_image/stb_image.h"

#include <GL/glew.h>

Texture::Texture(const std::string &path)
    : file_path(path)
{
    stbi_set_flip_vertically_on_load(1);
    local_buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4); // 4 = r, g, b, a

    GL_CALL(glGenTextures(1, &rendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (local_buffer)
        stbi_image_free(local_buffer);
}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1, &rendererID));
}

int Texture::getWidth() const
{
    return width;
}
int Texture::getHieght() const
{
    return height;
}
int Texture::getBPP() const
{
    return BPP;
}

void Texture::bind(const unsigned int slot) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::unBind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
