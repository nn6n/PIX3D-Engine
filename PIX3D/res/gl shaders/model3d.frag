#version 460 core

#extension GL_ARB_bindless_texture : require

// out
layout (location = 0) out vec4 FragColor;

// in
layout (location = 0) in vec3 in_Normal;
layout (location = 1) in vec2 in_TexCoords;

// storage buffer
layout(binding = 1, std430) readonly buffer MaterialsBuffer
{
   sampler2D base_color_texture[];
}material;

// uniforms
layout (location = 1) uniform float u_MeshIndex;
layout (location = 2) uniform float u_TilinhFactor;


void main()
{
    int mesh_index = int(u_MeshIndex);
    vec4 tex_color = texture(material.base_color_texture[mesh_index], in_TexCoords * u_TilinhFactor);
    FragColor = vec4(tex_color.rgb, 1.0);
}
