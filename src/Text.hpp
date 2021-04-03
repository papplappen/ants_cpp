#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

#define GLT_IMPLEMENTATION
#include <gltext.h>
#undef GLT_IMPLEMENTATION

#include <string>

class Text {
   public:
    Text(const std::string& text, glm::vec4 color);
    Text(const std::string& text, glm::vec3 color) : Text(text, glm::vec4(color, 1.0)){};
    Text(const std::string& text) : Text(text, glm::vec4(0.0, 0.0, 0.0, 1.0)){};
    Text() : Text(""){};

    GLTtext* glttext;
    glm::vec4 color;

    void setText(std::string text);
    void setColor(glm::vec4 color);
    void draw(GLfloat x, GLfloat y, GLfloat scale);

   private:
    static void init();
    static bool initialized;
};