#include "texture.h"
#include "../../vendors/stb/stb_image.h"
#include <iostream>

namespace kawa
{
	texture texture::create(const char* filepath)
	{
		return texture(filepath);
	}

	std::array<vec2, 4> texture::get_texture_coords() const
	{
		return std::array<vec2, 4>
		{
			vec2{ 0.0f, 0.0f },
			vec2{ 1.0f, 0.0f },
			vec2{ 1.0f, 1.0f },
			vec2{ 0.0f, 1.0f },
		};
	}

	std::array<vec2, 4> texture::get_sub_texture_coords(const vec2& block_size, const vec2& block_position) const
	{
		if (mirrored)
		{
			return std::array<vec2, 4>
			{
				vec2{ ((block_position.x + 1) * block_size.x) / _width, (block_position.y * block_size.y) / _height },
				vec2{ ((block_position.x * block_size.x)) / _width, (block_position.y * block_size.y) / _height },
				vec2{ (block_position.x * block_size.x) / _width, ((block_position.y + 1) * block_size.y) / _height },
				vec2{ ((block_position.x + 1) * block_size.x) / _width, ((block_position.y + 1) * block_size.y) / _height },
			};	
		}

		return std::array<vec2, 4>
		{
			vec2{ ((block_position.x * block_size.x)) / _width, (block_position.y * block_size.y) / _height },
			vec2{ ((block_position.x + 1) * block_size.x) / _width, (block_position.y * block_size.y) / _height },
			vec2{ ((block_position.x + 1) * block_size.x) / _width, ((block_position.y + 1) * block_size.y) / _height },
			vec2{ (block_position.x * block_size.x) / _width, ((block_position.y + 1) * block_size.y) / _height },
		};
	}

	uint8_t* texture::get_data() const
	{
		return _data;
	}

	void texture::bind()
	{
		//glBindTextureUnit(_slot, _id);
	}


	texture::texture(const char* filepath)
		:	mirrored(false)
	{
		_handle_idx = 0;

		stbi_set_flip_vertically_on_load(true);

		_data = stbi_load(filepath, &_width, &_height, &_col_chanels, 0);

		if (!_data)
		{
			std::cout << "ERROR: loading texture from " << filepath << " failed" << '\n';
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);	

	}

	texture::~texture()
	{											   
		glDeleteTextures(1, &_id);
	}


}