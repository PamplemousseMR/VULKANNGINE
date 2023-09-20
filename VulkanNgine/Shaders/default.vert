#version 450
#extension GL_ARB_separate_shader_objects : enable

vec2 f2Pos_Vs[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 f3VertexCol[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 v_f3VertexCol;

void main() {
    gl_Position = vec4(f2Pos_Vs[gl_VertexIndex], 0.0, 1.0);
    v_f3VertexCol = f3VertexCol[gl_VertexIndex];
}
