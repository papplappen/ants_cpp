#include "Main.hpp"

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#define GLT_IMPLEMENTATION
#include <gltext.h>

#include "Utils.hpp"
#include "GLUtils.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "Ant.hpp"
#include "Food.hpp"
#include "Home.hpp"

GLFWwindow *window;
glm::ivec2 viewport_size;

const float ANT_COMPSHADER_INT_SCALE = 1000.0;

std::forward_list<Ant> ants;
std::vector<AntGPUData> antsGpuData;
std::forward_list<Home> homes;
std::forward_list<Food> foods;

void updateAll() {
    for (Ant &a : ants) {
        a.update();
    }
    ants.remove_if(Ant::dead);

    foods.remove_if(Food::empty);

    for (Home h : homes) {
        h.update();
    }
}
void showAll() {
}

int main() {
    std::cout << "BADAMS!" << std::endl;
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

    for (int i = 0; i < 1000; i++) {
        ants.push_front(Ant(glm::diskRand(0.5f * viewport_size.y), glm::circularRand(1.0f), viewport_size, homes, foods, antsGpuData));
    }
    // ants.push_front(Ant(glm::vec2(100, 100), glm::circularRand(1.0f), viewport_size, homes, foods, antsGpuData));
    // ants.push_front(Ant(glm::vec2(100, -100), glm::circularRand(1.0f), viewport_size, homes, foods, antsGpuData));
    // ants.push_front(Ant(glm::vec2(-100, 100), glm::circularRand(1.0f), viewport_size, homes, foods, antsGpuData));
    // ants.push_front(Ant(glm::vec2(-100, -100), glm::circularRand(1.0f), viewport_size, homes, foods, antsGpuData));
    // for (AntGPUData a : antsGpuData) {
    //     std::cout << a.pos.x << " | " << a.pos.y << std::endl;
    // }

    GLuint vbo_ants;
    glGenBuffers(1, &vbo_ants);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ants);
    glBufferData(GL_ARRAY_BUFFER, vectorsizeof(antsGpuData), antsGpuData.data(), GL_DYNAMIC_DRAW);

    Texture tex_ants("res/ant2.png", true, true, true);

    Texture tex_simplex("res/noise2.png", false, true, false);
    tex_simplex.bindImageTexture(0, true, false);

    // GLuint tex_simplex;
    // glGenTextures(1, &tex_simplex);
    // glBindTexture(GL_TEXTURE_2D, tex_simplex);
    // {
    //     int width, height, channels;
    //     unsigned char *img = stbi_load("res/noise.png", &width, &height, &channels, 0);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    //     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glGenerateMipmap(GL_TEXTURE_2D);

    //     glBindImageTexture(0, tex_simplex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    //     stbi_image_free(img);
    // }
    // glBindTexture(GL_TEXTURE_2D, 0);

    ShaderProgram shader_ant("shaders/ant.vert", "shaders/ant.frag");
    shader_ant.point_attribute("pos", 2, GL_FLOAT, GL_FALSE, sizeof(AntGPUData), (void *)offsetof(AntGPUData, pos));
    shader_ant.point_attribute("dir", 2, GL_FLOAT, GL_FALSE, sizeof(AntGPUData), (void *)offsetof(AntGPUData, dir));
    shader_ant.set_uniformf("trans", glm::mat2(1.0f / (0.5f * viewport_size.x), 0, 0, 1.0f / (0.5f * viewport_size.y)));

    GLuint ssbo_newdirs;
    glGenBuffers(1, &ssbo_newdirs);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, antsGpuData.size() * sizeof(glm::ivec2), NULL, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_newdirs);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    GLuint ssbo_ants;
    glGenBuffers(1, &ssbo_ants);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ants);
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, vectorsizeof(antsGpuData), antsGpuData.data(), GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_ants);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // GLuint compshader_test = glCreateProgram();
    // {
    //     GLuint compshader_test_obj = compile_shader("shaders/test.comp", GL_COMPUTE_SHADER);
    //     glAttachShader(compshader_test, compshader_test_obj);
    //     glLinkProgram(compshader_test);
    //     check_shader_program_link_status(compshader_test, "Compshader");
    // }
    ShaderProgram compshader_test("shaders/test.comp");

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
            glBufferData(GL_ARRAY_BUFFER, vectorsizeof(antsGpuData), antsGpuData.data(), GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_ants);
            glBufferData(GL_SHADER_STORAGE_BUFFER, vectorsizeof(antsGpuData), antsGpuData.data(), GL_DYNAMIC_READ);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

            {
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
                {
                    glm::ivec2 zero = glm::ivec2(0, 0);
                    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_RG32I, GL_RG_INTEGER, GL_INT, &zero);
                }
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

                compshader_test.dispatch(antsGpuData.size(), 1, 1);

                glMemoryBarrier(GL_ALL_BARRIER_BITS);

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_newdirs);
                glm::ivec2 *newdirs = (glm::ivec2 *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
                {
                    for (int i = 0; i < antsGpuData.size(); i++) {
                        glm::vec2 newdir = glm::vec2(newdirs[i]) / ANT_COMPSHADER_INT_SCALE;
                        // std::cout << glm::to_string(newdir) << std::endl;
                        antsGpuData[i].dir += 0.4 * newdir;
                    }
                }
                glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            }
        }

        /* --- RENDER --- */
        {
            shader_ant.use();
            glBindBuffer(GL_ARRAY_BUFFER, vbo_ants);
            tex_ants.bind();
            {
                glDrawArrays(GL_POINTS, 0, antsGpuData.size());
            }
            tex_ants.unbind();
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