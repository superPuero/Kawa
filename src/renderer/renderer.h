#pragma once
#define renderer_buffer_size 0xFFFFFF
#define renderer_max_textures 0xFFF

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <unordered_map>
#include <array>
#include "../core/data_structures/dyn_array.h"

#include "glad/glad.h"
#include "glm.hpp"
#include "ext.hpp"

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Shader.h"
#include "texture.h"
#include "../utils/utils.h"
#include "../core/components/components.h"

namespace kawa
{
	struct vertex
	{
		glm::vec3 position;
		glm::vec2 texture_coords;
		int texture_id;
	};

	struct mesh 
	{
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class renderer
	{
	public:

		struct draw_command
		{

		};

		static void init();
		static void push_textured_quad(const transform& transform, const vec3& size, const vec3& offset, const texture& texture, const std::array<vec2, 4>& texture_coords);
		static void push_locked_textured_quad(const transform& transform, const vec3& size, const vec3& offset, const texture& texture, const std::array<vec2, 4>& texture_coords);
		static void push_colored_quad(const transform& transform, const vec3& size, const vec3& offset, const Color& color);
		static void draw_line(const transform& tr, const transform& tr_, float thickness, const Color& color);
		static bool load_mesh(const std::string& name, const std::string& path);
		static void process_node(aiNode* node, const aiScene* scene);
		static void process_mesh(aiMesh* mesh, const aiScene* scene);


		static void draw_mesh(const std::string& name);

		static void draw();
		static void shutdown();
		static void clear();
		static void load_texture(const char* filepath, const std::string& name, bool mirrored = false);
		static void unload_texture(const std::string& name);
		static void clear_color(Color color);

	public:
		inline static std::array<int32_t, 32> active_textures;
		inline static std::unordered_map<std::string, texture> textures;
		inline static std::unordered_map<std::string, mesh> meshes;
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