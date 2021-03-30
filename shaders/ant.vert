#version 330

const float POINT_SIZE = 16.0;

in vec2 pos;
in vec2 dir;

out vec2 Dir;

void main() {
    Dir = dir;
    gl_PointSize = POINT_SIZE;
    gl_Position = vec4(pos, 0.0, 1.0);
}
