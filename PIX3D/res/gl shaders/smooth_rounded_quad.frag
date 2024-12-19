#version 460 core

layout (location = 0) out vec4 FragColor;

// in
layout (location = 0) in vec2 in_coords;
layout (location = 1) in vec2 in_uvs;

// uniforms
layout (location = 1) uniform vec4 u_color;
layout (location = 2) uniform float u_smoothness;
layout (location = 3) uniform float u_corner_radius;
layout (location = 4) uniform float u_use_texture;
layout (location = 5) uniform sampler2D u_texture;

void main()
{
    // Translate to square's local space
    vec2 localPos = abs(in_coords - vec2(0.0)) - (1.0 - u_corner_radius);

    // Compute the SDF distance for a rounded square
    float dist = length(max(localPos, 0.0)) - u_corner_radius;

    // Smooth edge blending
    float alpha = smoothstep(0.0, u_smoothness, -dist);

    vec4 color = u_color;
    
    if(u_use_texture == 1.0)
    {
       color *= texture(u_texture, in_uvs);
    }

    // Set fragment color with smooth alpha blending
    FragColor = vec4(color.rgb, color.a * alpha);
}
