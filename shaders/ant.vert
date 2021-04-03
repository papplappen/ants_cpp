#version 330

const float POINT_SIZE = 16.0;

uniform mat2 transform;
uniform vec2 translate;

in vec2 pos;
in vec2 dir;

out vec2 Dir;

void main() {
    Dir = dir;
    gl_PointSize = POINT_SIZE;
    gl_Position = vec4(transform * (pos + translate), 0.0, 1.0);
}
