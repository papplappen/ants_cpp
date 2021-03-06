#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const std::string &vertex_shader_file, const std::string &fragment_shader_file, const std::string &out_color_name) {
    gl_name = compiler_render_program(vertex_shader_file, fragment_shader_file, out_color_name);
    glGenVertexArrays(1, &vao);
}

ShaderProgram::ShaderProgram(const std::string &compute_shader_file) {
    gl_name = glCreateProgram();
    GLuint compshader_obj = compile_shader(compute_shader_file, GL_COMPUTE_SHADER);
    glAttachShader(gl_name, compshader_obj);
    glLinkProgram(gl_name);
    check_shader_program_link_status(gl_name, "\"" + compute_shader_file + "\"");
}

void ShaderProgram::use() {
    glBindVertexArray(vao);
    glUseProgram(gl_name);
}

void ShaderProgram::unuse() {
    glUseProgram(0);
}

void ShaderProgram::dispatch(GLuint x, GLuint y, GLuint z) {
    glUseProgram(gl_name);
    glDispatchCompute(x, y, z);
    glUseProgram(0);
}

GLuint ShaderProgram::point_attribute(const std::string &attribute_name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *data) {
    glBindVertexArray(vao);
    glUseProgram(gl_name);
    GLint loc = glGetAttribLocation(gl_name, attribute_name.c_str());
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, size, type, normalized, stride, data);
    glBindVertexArray(0);
    glUseProgram(0);
    return loc;
}

GLint ShaderProgram::get_uniform_location(const std::string &uniform_name) {
    auto find = uniform_locations.find(uniform_name);
    GLint loc;
    if (find == uniform_locations.end()) {
        loc = glGetUniformLocation(gl_name, uniform_name.c_str());
        uniform_locations[uniform_name] = loc;
    } else {
        loc = find->second;
    }
    return loc;
}

void ShaderProgram::set_uniformf(const std::string &uniform_name, float f) {
    glUseProgram(gl_name);
    glUniform1f(get_uniform_location(uniform_name), f);
    glUseProgram(0);
}
void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::vec2 v) {
    glUseProgram(gl_name);
    glUniform2fv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::vec3 v) {
    glUseProgram(gl_name);
    glUniform3fv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::vec4 v) {
    glUseProgram(gl_name);
    glUniform4fv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}

void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::mat2 m) {
    glUseProgram(gl_name);
    glUniformMatrix2fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(0);
}
void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::mat3 m) {
    glUseProgram(gl_name);
    glUniformMatrix3fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(0);
}
void ShaderProgram::set_uniformf(const std::string &uniform_name, glm::mat4 m) {
    glUseProgram(gl_name);
    glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(m));
    glUseProgram(0);
}

void ShaderProgram::set_uniformi(const std::string &uniform_name, int v) {
    glUseProgram(gl_name);
    glUniform1i(get_uniform_location(uniform_name), v);
    glUseProgram(0);
}
void ShaderProgram::set_uniformi(const std::string &uniform_name, glm::ivec2 v) {
    glUseProgram(gl_name);
    glUniform2iv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformi(const std::string &uniform_name, glm::ivec3 v) {
    glUseProgram(gl_name);
    glUniform3iv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformi(const std::string &uniform_name, glm::ivec4 v) {
    glUseProgram(gl_name);
    glUniform4iv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}

void ShaderProgram::set_uniformu(const std::string &uniform_name, unsigned int v) {
    glUseProgram(gl_name);
    glUniform1ui(get_uniform_location(uniform_name), v);
    glUseProgram(0);
}
void ShaderProgram::set_uniformu(const std::string &uniform_name, glm::uvec2 v) {
    glUseProgram(gl_name);
    glUniform2uiv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformu(const std::string &uniform_name, glm::uvec3 v) {
    glUseProgram(gl_name);
    glUniform3uiv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}
void ShaderProgram::set_uniformu(const std::string &uniform_name, glm::uvec4 v) {
    glUseProgram(gl_name);
    glUniform4uiv(get_uniform_location(uniform_name), 1, glm::value_ptr(v));
    glUseProgram(0);
}