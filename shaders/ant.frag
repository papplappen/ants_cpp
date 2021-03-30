#version 330

uniform sampler2D tex;

in vec2 Dir;

out vec4 out_color;

void main() {
    // out_color = vec4(1.0, 0.0, 0.0, 1.0);
    vec2 uv = gl_PointCoord - vec2(0.5);
    uv = mat2(Dir.y, -Dir.x, Dir.x, Dir.y) * uv;
    uv = uv + vec2(0.5);
    out_color = texture(tex, uv);
}
