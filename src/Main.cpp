#include "Main.hpp"

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLT_IMPLEMENTATION
#include <gltext.h>

#include "Utils.hpp"
#include "GLUtils.hpp"
#include "ShaderProgram.hpp"

GLFWwindow *window;
glm::ivec2 viewport_size;

const float ANT_COMPSHADER_INT_SCALE = 1000.0;

struct Ant {
    glm::vec2 pos;
    glm::vec2 dir;

    void update() {
        dir += 0.5 * glm::circularRand(1.0f);
        dir = glm::normalize(dir);
        pos += 0.003 * dir;
        if (pos.x > 1) {
            pos.x = 1;
            dir.x = -dir.x;
        } else if (pos.x < -1) {
            pos.x = -1;
            dir.x = -dir.x;
        }
        if (pos.y > 1) {
            pos.y = 1;
            dir.y = -dir.y;
        } else if (pos.y < -1) {
            pos.y = -1;
            dir.y = -dir.y;
        }
    }
};

std::vector<Ant> ants;

int main() {
    //GL SETUP
    window = init_glfw("蟻");
    init_glew();

    gltInit();
    GLTtext *text_fps = gltCreateText();

    glClearColor(0.85, 0.75, 0.55, 1.0);

    glfwGetFramebufferSize(window, &viewport_size.x, &viewport_size.y);

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    for (int i = 0; i < 10000; i++) {
        ants.push_back(Ant{glm::diskRand(1.0f), glm::circularRand(1.0f)});
    }

    GLuint vbo_ants;
    glGenBuffers(1, &vbo_ants);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ants);
    glBufferData(GL_ARRAY_BUFFER, vectorsizeof(ants), ants.data(), GL_DYNAMIC_DRAW);

    GLuint tex_ant;
    glGenTextures(1, &tex_ant);
    glBindTexture(GL_TEXTURE_2D, tex_ant);
    {
        int width, height, channels;
        unsigned char *img = stbi_load("res/ant2.png", &width, &height, &channels, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(img);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex_simplex;
    glGenTextures(1, &tex_simplex);
    glBindTexture(GL_TEXTURE_2D, tex_simplex);
    {
        int width, height, channels;
        unsigned char *img = stbi_load("res/noise.png", &width, &height, &channels, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glGenerateMipmap(GL_TEXTURE_2D);

        glBindImageTexture(0, tex_simplex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

        stbi_image_free(img);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    ShaderProgram shader_ant("shaders/ant");
    shader_ant.point_attribute("pos", 2, GL_FLOAT, GL_FALSE, sizeof(Ant), (void *)offsetof(Ant, pos));
    shader_ant.point_attribute("dir", 2, GL_FLOAT, GL_FALSE, sizeof(Ant), (void *)offsetof(Ant, dir));

    GLuint ssbo_newdirs;
    glGenBuffers(1, &ssbo_newdirs);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, ants.size() * sizeof(glm::ivec2), NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_newdirs);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    GLuint ssbo_ants;
    glGenBuffers(1, &ssbo_ants);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ants);
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, vectorsizeof(ants), ants.data(), GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_ants);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    GLuint compshader_test = glCreateProgram();
    {
        GLuint compshader_test_obj = compile_shader("shaders/test.comp", GL_COMPUTE_SHADER);
        glAttachShader(compshader_test, compshader_test_obj);
        glLinkProgram(compshader_test);
        check_shader_program_link_status(compshader_test, "Compshader");
    }

    double start_time = glfwGetTime();
    double last_frame_time = start_time;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update time & deltatime
        double frame_time = glfwGetTime();
        double time = frame_time - start_time;
        double deltatime = frame_time - last_frame_time;
        last_frame_time = frame_time;

        /* --- UPDATE --- */
        {
            for (Ant &a : ants) {
                a.update();
            }
            glBindBuffer(GL_ARRAY_BUFFER, vbo_ants);
            glBufferData(GL_ARRAY_BUFFER, vectorsizeof(ants), ants.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ants);
            glBufferData(GL_SHADER_STORAGE_BUFFER, vectorsizeof(ants), ants.data(), GL_DYNAMIC_READ);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

            glUseProgram(compshader_test);
            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
                {
                    glm::ivec2 zero = glm::ivec2(0, 0);
                    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_RG32I, GL_RG_INTEGER, GL_INT, &zero);
                }
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

                glDispatchCompute(ants.size(), 1, 1);

                glMemoryBarrier(GL_ALL_BARRIER_BITS);

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
                glm::ivec2 *newdirs = (glm::ivec2 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
                {
                    for (int i = 0; i < ants.size(); i++) {
                        glm::vec2 newdir = glm::vec2(newdirs[i]) / ANT_COMPSHADER_INT_SCALE;
                        // std::cout << glm::to_string(newdir) << std::endl;
                        ants[i].dir += 0.4 * newdir;
                    }
                }
                glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            }
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }

        /* --- RENDER --- */
        {
            shader_ant.use();
            glBindBuffer(GL_ARRAY_BUFFER, vbo_ants);
            glBindTexture(GL_TEXTURE_2D, tex_ant);
            {
                glDrawArrays(GL_POINTS, 0, ants.size());
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /* --- TEXT --- */
        {
            gltSetText(text_fps, std::to_string(int(1 / deltatime)).c_str());
            gltBeginDraw();
            gltColor(1.0f, 1.0f, 1.0f, 1.0f);
            gltDrawText2D(text_fps, 0, 0, 1.0);
            gltEndDraw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwTerminate();
        exit(0);
    }
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
}