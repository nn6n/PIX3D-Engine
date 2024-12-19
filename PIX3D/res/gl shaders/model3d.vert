#version 460 core

#extension GL_ARB_bindless_texture : require

// in
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;

// out
layout (location = 0) out vec3 out_Normal;
layout (location = 1) out vec2 out_TexCoords;

// uniform buffer
layout (binding = 0, std140) uniform u_camera
{
     mat4 proj;
}cam;

// uniforms
layout (location = 0) uniform mat4 model;

void main()
{
    out_Normal = in_Normal;
    out_TexCoords = in_TexCoords;
    gl_Position = cam.proj * model * vec4(in_Position, 1.0);
}
