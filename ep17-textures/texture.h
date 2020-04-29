#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <GL/glew.h>

#define GlCheckErrors() {int line = __LINE__; while (GLenum error_code = glGetError()) { \
        printf("[Error - OpenGL]\n");   \
        printf("\tFile: %s\n", __FILE__); \
        printf("\tLine: %d\n", line); \
        printf("\tError code: %d\n", error_code); \
        exit(-3); \
        } \
    }

class Texture {
private:
    unsigned int renderer_id;
    std::string file_path;
    unsigned char* local_buffer;
    int width, height, bpp;
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const {return width;};
    inline int GetHeight() const {return height;};
};

#endif // TEXTURE_H