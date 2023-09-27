#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 f2Pos_Vs;
layout(location = 1) in vec3 f3VertexCol;

layout(location = 0) out vec3 v_f3VertexCol;

void main() {
    gl_Position = vec4(f2Pos_Vs, 0.0, 1.0);
    v_f3VertexCol = f3VertexCol;
}
