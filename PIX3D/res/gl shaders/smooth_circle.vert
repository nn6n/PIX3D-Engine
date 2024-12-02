#version 460 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_coords;


// uniform
layout (location = 0) uniform mat4 u_Projection;

// out
layout (location = 0) out vec2 out_coords;

void main()
{
    out_coords = in_coords;
    gl_Position = u_Projection * vec4(in_position, 1.0);
}
