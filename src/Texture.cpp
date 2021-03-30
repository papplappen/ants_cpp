#include "Texture.hpp"

Texture::Texture(const std::string& texture_file, bool mipmap, bool filter_linear, bool wrap_clamp) {
    glGenTextures(1, &gl_name);
    glBindTexture(GL_TEXTURE_2D, gl_name);

    int width, height, channels = 0;
    const char *filename_cstr = texture_file.c_str();
    unsigned char *img = stbi_load(filename_cstr, &width, &height, &channels, 0);
    GLenum format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        std::cerr << "Failed loading image \"" << texture_file << "\":" << std::endl;
        std::cerr << "  width    = " << width << std::endl;
        std::cerr << "  height   = " << height << std::endl;
        std::cerr << "  channels = " << channels << std::endl;
        stbi_image_free(img);
        exit(-1);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, format, GL_UNSIGNED_BYTE, img);
    stbi_image_free(img);

    if (wrap_clamp) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    if (filter_linear) {
        if (mipmap) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_name);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindImageTexture(GLuint unit, bool read, bool write) {
    GLenum access = (read && !write ? GL_READ_ONLY : (!read && write ? GL_WRITE_ONLY : GL_READ_WRITE));
    glBindImageTexture(unit, gl_name, 0, GL_FALSE, 0, access, GL_RGBA32F);
}