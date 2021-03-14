#pragma once

#include "renderer.hpp"

class Texture
{
private:
    unsigned int rendererID = 0;
    std::string file_path;
    unsigned char *local_buffer;
    int width = 0, height = 0, BPP = 0; // Bits per pixel

public:
    Texture(const std::string &path);
    ~Texture();

    int getWidth () const;
    int getHieght() const;
    int getBPP   () const;

    void   bind(const unsigned int slot = 0) const;
    void unBind() const;
};
