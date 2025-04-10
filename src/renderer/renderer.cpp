#include <new>

#include "renderer.h"
#include "../debug/debug.h"

namespace kawa
{
	void renderer::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_data_buffer = new vertex[4 * renderer_buffer_size];
		_index_buffer = new uint32_t[6 * renderer_buffer_size];
		_texture_handles = new uint64_t[renderer_max_textures];

		std::memset(_texture_handles, (uint64_t)-1, renderer_max_textures * sizeof(uint64_t));

		_data_occupied = 0;
		_index_occupied = 0;

		shader_map["test"].create("shaders/test_vert.vert", "shaders/better.frag");
		shader_map["test"].use();
	
		_vao.create();
		_vao.bind();

		_vbo.create();
		_vbo.bind();

		_vao.set_attrib_ptr(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
		_vao.enable_attrib_array(0);

		_vao.set_attrib_ptr(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, texture_coords)));
		_vao.enable_attrib_array(1);

		_vao.set_attrib_int_ptr(2, 1, GL_INT, GL_FALSE, sizeof(vertex), (void*)(offsetof(vertex, texture_id)));
		_vao.enable_attrib_array(2);

		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER,
			sizeof(uint64_t) * renderer_max_textures,
			nullptr,
			GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

		_vbo.unbind();
		_vao.unbind();

		_ebo.create();

		renderer::load_texture("assets/white_test.png", "white");
		renderer::load_texture("assets/font/monogram-bitmap.png", "test_font");

	}
	void renderer::push_textured_quad(const transform& transform, const vec3& size, const vec3& offset, const texture& texture, const std::array<vec2, 4>& texture_coords)
	{

		glm::mat4 mat_transform = transform.get_mat4();

		auto data_ptr = _data_buffer + _data_occupied;
		auto index_ptr = _index_buffer + _index_occupied;


		new(data_ptr + 0) vertex{ mat_transform * glm::vec4{ + offset.x, + offset.y, + offset.z, 1 }, texture_coords[0], texture._handle_idx };
		new(data_ptr + 1) vertex{ mat_transform * glm::vec4{ + size.x + offset.x, + offset.y, + offset.z, 1 }, texture_coords[1], texture._handle_idx };
		new(data_ptr + 2) vertex{ mat_transform * glm::vec4{ + size.x + offset.x, + size.y + offset.y, + size.z + offset.z, 1 }, texture_coords[2], texture._handle_idx };
		new(data_ptr + 3) vertex{ mat_transform * glm::vec4{ + offset.x, + size.y + offset.y, + size.z + offset.z, 1 }, texture_coords[3], texture._handle_idx };

		_data_occupied += 4;

		new(index_ptr + 0) uint32_t(0 + _data_occupied - 4);
		new(index_ptr + 1) uint32_t(2 + _data_occupied - 4);
		new(index_ptr + 2) uint32_t(1 + _data_occupied - 4);
		new(index_ptr + 3) uint32_t(0 + _data_occupied - 4);
		new(index_ptr + 4) uint32_t(3 + _data_occupied - 4);
		new(index_ptr + 5) uint32_t(2 + _data_occupied - 4);

		_index_occupied += 6;
	};

	void renderer::push_colored_quad(const transform& transform, const vec3& size, const vec3& offset, const Color& color)
	{
		glm::mat4 mat_transform = transform.get_mat4();

		auto data_ptr = _data_buffer + _data_occupied;
		auto index_ptr = _index_buffer + _index_occupied;


		new(data_ptr + 0) vertex{ mat_transform * glm::vec4{ +offset.x, +offset.y, +offset.z, 1 }, vec2{ 0.0f, 0.0f}, 0 };
		new(data_ptr + 1) vertex{ mat_transform * glm::vec4{ +size.x + offset.x, +offset.y, +offset.z, 1 }, vec2{ 1.0f, 0.0f }, 0 };
		new(data_ptr + 2) vertex{ mat_transform * glm::vec4{ +size.x + offset.x, +size.y + offset.y, +size.z + offset.z, 1 }, vec2{ 1.0f, 1.0f }, 0 };
		new(data_ptr + 3) vertex{ mat_transform * glm::vec4{ +offset.x, +size.y + offset.y, +size.z + offset.z, 1 }, vec2{ 0.0f, 1.0f }, 0 };

		_data_occupied += 4;

		new(index_ptr + 0) uint32_t(0 + _data_occupied - 4);
		new(index_ptr + 1) uint32_t(2 + _data_occupied - 4);
		new(index_ptr + 2) uint32_t(1 + _data_occupied - 4);
		new(index_ptr + 3) uint32_t(0 + _data_occupied - 4);
		new(index_ptr + 4) uint32_t(3 + _data_occupied - 4);
		new(index_ptr + 5) uint32_t(2 + _data_occupied - 4);

		_index_occupied += 6;
	}


void renderer::draw()
	{


		_vao.bind();

		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

		_vbo.bind();
		_vbo.data(GL_ARRAY_BUFFER, _data_occupied * sizeof(vertex), _data_buffer, GL_DYNAMIC_DRAW);

		_ebo.bind();
		_ebo.data(sizeof(uint32_t), _index_occupied, _index_buffer);

		_ebo.draw();

		_vbo.unbind();
		_ebo.unbind();
		_vao.unbind();

		_data_occupied = 0;
		_index_occupied = 0;
	}

	void renderer::shutdown()
	{
		if (_data_buffer)
		{
			delete[] _data_buffer;
			_data_buffer = nullptr;
		}

		if (_index_buffer)
		{			
			delete[] _index_buffer;
			_index_buffer = nullptr;
		}


	}

	void renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void renderer::load_texture(const char* filepath, const std::string& name, bool mirrored)
	{		
		for (uint64_t i = 0; i < renderer_max_textures; i++)
		{
			if (_texture_handles[i] == -1)
			{
				textures.emplace(name, filepath);
				
				if(textures[name]._data)
				{
					uint64_t handle = glGetTextureHandleARB(textures[name]._id);
					glMakeTextureHandleResidentARB(handle);

					_texture_handles[i] = handle;
					textures[name]._handle_idx = i;
					textures[name]._name = name;

					glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
					glBufferSubData(GL_SHADER_STORAGE_BUFFER,
						i * sizeof(uint64_t),
						sizeof(uint64_t),
						&handle);

					KW_LOG("Loaded texture", name, "with ID:", textures[name]._id, "into slot", textures[name]._handle_idx);
				}
				else
				{
					KW_LOG_ERROR("Unable to load texture", name);
				}

				return;
			}
		}
	}

	void renderer::unload_texture(const std::string& name)
	{
		glMakeTextureHandleNonResidentARB(textures[name]._handle_idx);

		uint64_t zero = 0;
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER,
			textures[name]._handle_idx * sizeof(uint64_t),
			sizeof(uint64_t),
			&zero);

		_texture_handles[textures[name]._handle_idx] = (uint64_t) - 1;

		textures.erase(name);

		KW_LOG("Unloaded texture");

	}

	void renderer::clear_color(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

}
