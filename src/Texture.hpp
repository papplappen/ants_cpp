#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <string>
#include <iostream>

class Texture {
   public:
    Texture(std::string filename, bool mipmap, bool clamp);
    Texture(std::string filename) : Texture(filename, true, false){};
    GLuint gl_name;

    void bind();
    void unbind();

   private:
};