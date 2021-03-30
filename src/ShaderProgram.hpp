#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include <unordered_map>

#include "GLUtils.hpp"

class ShaderProgram {
   public:
    ShaderProgram(const std::string& vertex_shader_file, const std::string& fragment_shader_file, const std::string& out_color_name);
    ShaderProgram(const std::string& vertex_shader_file, const std::string& fragment_shader_file) : ShaderProgram(vertex_shader_file, fragment_shader_file, "out_color"){};
    ShaderProgram(const std::string& compute_shader_file);
    GLuint gl_name;
    GLuint vao = 0;

    void use();
    void unuse();
    void dispatch(GLuint x, GLuint y, GLuint z);

    GLuint point_attribute(const std::string& attribute_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data);
    GLint get_uniform_location(const std::string& uniform_name);

    void set_uniformf(const std::string& uniform_name, float f);
    void set_uniformf(const std::string& uniform_name, glm::vec2 v);
    void set_uniformf(const std::string& uniform_name, glm::vec3 v);
    void set_uniformf(const std::string& uniform_name, glm::vec4 v);
    void set_uniformf(const std::string& uniform_name, glm::mat2 m);
    void set_uniformf(const std::string& uniform_name, glm::mat3 m);
    void set_uniformf(const std::string& uniform_name, glm::mat4 m);
    void set_uniformi(const std::string& uniform_name, int n);
    void set_uniformi(const std::string& uniform_name, glm::ivec2 v);
    void set_uniformi(const std::string& uniform_name, glm::ivec3 v);
    void set_uniformi(const std::string& uniform_name, glm::ivec4 v);
    void set_uniformu(const std::string& uniform_name, unsigned int n);
    void set_uniformu(const std::string& uniform_name, glm::uvec2 v);
    void set_uniformu(const std::string& uniform_name, glm::uvec3 v);
    void set_uniformu(const std::string& uniform_name, glm::uvec4 v);

   private:
    std::unordered_map<std::string, GLint> uniform_locations;
};