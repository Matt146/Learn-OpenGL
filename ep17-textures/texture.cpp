#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path) 
    : file_path(path), local_buffer(nullptr), width(0), height(0), bpp(0)
{
    std::cout << "a" << std::endl;
    GlCheckErrors();
    // load the texture
    stbi_set_flip_vertically_on_load(1);
    local_buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
    GlCheckErrors();

    // generate texture
    GlCheckErrors();
    glGenTextures(1, &renderer_id);
    glBindTexture(GL_TEXTURE_2D, renderer_id);
    GlCheckErrors();

    // set texture parameters
    GlCheckErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GlCheckErrors();

    // give OpenGL texture data
    GlCheckErrors();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);
    GlCheckErrors();

    GlCheckErrors();
    if (local_buffer) {
        stbi_image_free(local_buffer);
    }
    GlCheckErrors();
}

Texture::~Texture() {
    GlCheckErrors();
    glDeleteTextures(1, &renderer_id);
    GlCheckErrors();
}

void Texture::Bind(unsigned int slot) const {
    GlCheckErrors();
    glBindTexture(GL_TEXTURE_2D, renderer_id);
    glActiveTexture(GL_TEXTURE0 + slot);
    GlCheckErrors();
}

void Texture::Unbind() const {
    GlCheckErrors();
    glBindTexture(GL_TEXTURE_2D, 0);
    GlCheckErrors();
}
