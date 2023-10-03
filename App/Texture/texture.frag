#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D s_texture;

layout(location = 0) in vec3 v_f3FragCol;
layout(location = 1) in vec2 v_f2FragTextCoord;

layout(location = 0) out vec4 f4OutCol;

void main() {
    f4OutCol = texture(s_texture, v_f2FragTextCoord);
}
