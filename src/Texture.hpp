#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include <iostream>

class Texture {
   public:
    Texture(const std::string& texture_file, bool mipmap, bool filter_linear, bool wrap_clamp);
    Texture(const std::string& texture_file) : Texture(texture_file, true, true, false){};
    GLuint gl_name;
    void bind();
    void unbind();
    void bindImageTexture(GLuint unit, bool read, bool write);

   private:
};