#pragma once
#include <cstdint>
#include <array>

#include "glad/glad.h"
#include "glm.hpp"
#include "../utils/utils.h"

namespace kawa
{
	class texture
	{
	public:
		texture() = default;
		texture(const char* filepath);
		~texture();

	public:
		static  texture				create(const char* filepath);

	public:
		uint8_t*					get_data() const;
		std::array<vec2, 4>			get_texture_coords() const;
		std::array<vec2, 4>			get_sub_texture_coords(const vec2& block_size, const vec2& block_position) const;
		void						bind();

	public:
		int32_t _width;
		int32_t _height;
		int32_t _col_chanels;
		uint8_t* _data;

		uint32_t _id;
		int32_t _handle_idx;

		std::string _name;

		bool mirrored;
	};

}
