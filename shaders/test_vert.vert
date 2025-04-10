#version 460 core

#extension GL_ARB_bindless_texture : require

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aTextureHandleIndex;

//uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

out vec2 TexCoord;
flat out int TextureHandleIndex;

void main()
{
	gl_Position = proj * view * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	TextureHandleIndex = aTextureHandleIndex;
}
	