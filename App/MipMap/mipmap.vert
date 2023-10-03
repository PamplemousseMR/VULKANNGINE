#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 m4ModelMs_Ls;
    mat4 m4ViewVs_Ms;
    mat4 m4ProjCs_Vs;
} u_mvp;

layout(location = 0) in vec3 f3Pos_Vs;
layout(location = 1) in vec3 f3VertexCol;
layout(location = 2) in vec2 f2VertexTextCoord;

layout(location = 0) out vec3 v_f3VertexCol;
layout(location = 1) out vec2 v_f2VertexTextCoord;

void main() {
    gl_Position = u_mvp.m4ProjCs_Vs * u_mvp.m4ViewVs_Ms * u_mvp.m4ModelMs_Ls * vec4(f3Pos_Vs, 1.0);
    v_f3VertexCol = f3VertexCol;
    v_f2VertexTextCoord = f2VertexTextCoord;
}
