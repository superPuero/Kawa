#include "Shader.h"
#include "../utils/utils.h"

namespace kawa
{

	Shader::Shader()
	{
	}

	void Shader::create(const char* vert_source_path, const char* frag_source_path)
	{
		std::string vert_source = read_to_str(vert_source_path);
		const char* c_vert_source = vert_source.c_str();
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &c_vert_source, NULL);
		glCompileShader(vertexShader);

		std::string frag_source = read_to_str(frag_source_path);
		const char* c_frag_source = frag_source.c_str();
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &c_frag_source, NULL);
		glCompileShader(fragmentShader);

		index = glCreateProgram();

		glAttachShader(index, vertexShader);
		glAttachShader(index, fragmentShader);

		glLinkProgram(index);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::uniform_mat4(const char* uniform_name, const GLfloat* data)
	{
		GLint location = glGetUniformLocation(index, uniform_name);

		if (location == -1)
		{
			std::cout << "no such uiniform with name " << uniform_name << " was found in shader" << '\n';
			__debugbreak();
		}

		glUniformMatrix4fv(location, 1, GL_FALSE, data);
	}

	void Shader::uniform_int(const char* uniform_name, GLint data)
	{
		GLint location = glGetUniformLocation(index, uniform_name);

		if (location == -1)
		{
			std::cout << "no such uniform " << uniform_name << '\n';
			__debugbreak();
		}
		glUniform1i(location, data);
	}

	void Shader::use()
	{
		glUseProgram(index);
	}

	void Shader::unuse()
	{
		glUseProgram(0);
	}

}