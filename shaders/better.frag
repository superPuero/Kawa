#version 460 core
#extension GL_ARB_bindless_texture : require

layout(std430, binding = 0) readonly buffer TextureHandles {
    uvec2 Handles[]; 
};

in vec2 TexCoord;
flat in int TextureHandleIndex;

out vec4 FragColor;

void main()
{
    sampler2D tex = sampler2D(Handles[TextureHandleIndex]);
    FragColor = texture(tex, TexCoord);
}