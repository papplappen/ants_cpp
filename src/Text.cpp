#include "Text.hpp"

Text::Text(const std::string& text, glm::vec4 color) {
    Text::init();
    glttext = gltCreateText();
    setText(text);
    setColor(color);
}

void Text::setText(std::string text) {
    gltSetText(glttext, text.c_str());
}

void Text::setColor(glm::vec4 color) {
    this->color = color;
}

void Text::draw(GLfloat x, GLfloat y, GLfloat scale) {
    gltBeginDraw();
    gltColor(color.r, color.g, color.b, color.a);
    gltDrawText2D(glttext, x, y, scale);
    gltEndDraw();
}

bool Text::initialized = false;

void Text::init() {
    if (!initialized) {
        initialized = true;
        gltInit();
    }
}