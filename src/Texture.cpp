#include "Texture.hpp"

Texture::Texture(std::string filename, bool mipmap, bool clamp) {

    glGenTextures(1, &gl_name);
    glBindTexture(GL_TEXTURE_2D, gl_name);

    int width, height, channels = 0;
    const char *filename_cstr = filename.c_str();
    unsigned char *img = stbi_load(filename_cstr, &width, &height, &channels, 0);
    GLenum format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        std::cerr << "Failed loading image \"" << filename << "\":" << std::endl;
        std::cerr << "  width    = " << width << std::endl;
        std::cerr << "  height   = " << height << std::endl;
        std::cerr << "  channels = " << channels << std::endl;
        stbi_image_free(img);
        exit(-1);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, img);
    stbi_image_free(img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_name);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}