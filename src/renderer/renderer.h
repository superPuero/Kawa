#pragma once
#define renderer_buffer_size 0xFFFFFF
#define renderer_max_textures 0xFFF

#include <unordered_map>
#include <array>

#include "glad/glad.h"
#include "glm.hpp"
#include "ext.hpp"

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "texture.h"
#include "../utils/utils.h"
#include "../core/components.h"


namespace kawa
{

	class renderer
	{
	public:
		static void init();
		static void push_true_quad(const texture& texture, const std::array<vec2, 4>& texture_coords, const vec3& size, const vec3& offset, const transform& transform);
		static void push_textured_quad(const texture& texture, const std::array<vec2, 4>& texture_coords, const vec3& pos, const vec3& offset);
		static void push_true_colored_quad(const transform& transform, const vec3& size, const vec3& offset, const Color& color);
		static void push_colored_quad(const Color& color, const vec3& pos, const vec3& offset);
		static void push_colored_quad_2d(const Color& color, const vec2& pos, const vec2& offset);

		static void draw();
		static void shutdown();
		static void clear();
		static void load_texture(const char* filepath, const std::string& name, bool mirrored = false);
		static void unload_texture(const std::string& name);
		static void clear_color(Color color);

	public:
		inline static std::array<int32_t, 32> active_textures;
		inline static std::unordered_map<std::string, texture> textures;
		inline static std::unordered_map<std::string, Shader> shader_map;

	private:
		inline static VAO _vao;
		inline static VBO _vbo;
		inline static EBO _ebo;

		inline static GLuint ssbo;

		inline static size_t _data_occupied;
		inline static size_t _index_occupied;
		inline static Color _clear_color;

		inline static uint64_t* _texture_handles;

	private:
		inline static vertex* _data_buffer;
		inline static uint32_t* _index_buffer;
	};

}