#pragma once
#include "glad/glad.h"

namespace kawa
{
	class Shader
	{
	public:
		Shader();
		void create(const char* vert_source_path, const char* frag_source_path);
		void uniform_mat4(const char* uniform_name, const GLfloat* data);
		void uniform_int(const char* uniform_name, GLint data);

	public:
		void use();
		void unuse();
	
	public:
		GLuint index;
	};
}
