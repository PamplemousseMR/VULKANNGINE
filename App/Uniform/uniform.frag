#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 v_f3FragCol;

layout(location = 0) out vec4 f4OutCol;

void main() {
    f4OutCol = vec4(v_f3FragCol, 1.0);
}
