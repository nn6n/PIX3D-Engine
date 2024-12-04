#version 460 core

layout (location = 0) out vec4 FragColor;

// in
layout (location = 0) in vec2 in_coords;

// uniforms
uniform vec4 u_color;

void main()
{
    vec4 color = u_color;

    // Compute distance from the center
    float distanceFromCenter = length(in_coords);

    if(distanceFromCenter >= 1.0) discard;

    FragColor = color;
}
