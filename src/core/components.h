#pragma once

#include <vector>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "macros.h"

#include "../utils/utils.h"
#include "../renderer/texture.h"

#include "../debug/debug.h"

#include "../scene/entity.h"

namespace kawa
{
	struct transform
	{
		vec3 position = { 0,0,0 };
		vec3 rotation = { 0,0,0 };
		vec3 scale = { 1.0f, 1.0f, 1.0f };

		inline glm::mat4 get_mat4() const noexcept
		{
			glm::mat4 model(1.0f);

			model = glm::translate(model, position);

			model = model * glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
			model = model * glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
			model = model * glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));

			model = glm::scale(model, scale);

			return model;
		}
	};

	struct text_component
	{
		std::string data;
		vec2 size;
	};

	struct sprite2d
	{
		texture* tex;
		vec2 size;
		vec2 offset;
		std::array<vec2, 4> texture_coords;

		inline void make_centred()
		{
			offset = { -size.x / 2, -size.y / 2 };
		}

		inline texture& get_texture() noexcept
		{
			if (tex)
			{
				return *tex;
			}
			else
			{
				KW_LOG_ERROR("No texture in sprite");
			}
		}
	};

	struct sprite2d_bundle
	{

		sprite2d_bundle() = default;

		sprite2d_bundle(size_t s)
			: size(s)
		{
			bundle = static_cast<sprite2d*>(std::malloc(sizeof(sprite2d) * size));
			current = 0;
		}

		~sprite2d_bundle()
		{
			if (bundle)
			{
				std::free(bundle);
				bundle = nullptr;
			}
		}

		inline sprite2d_bundle& put(
			size_t id,
			texture* tex,
			const vec2& size,
			const vec2& offset,
			const std::array<vec2, 4>& texture_coords
			)
		{
			new(bundle + id) sprite2d{ tex, size, offset, texture_coords };

			return *this;
		}

		inline sprite2d_bundle& put_centered(
			size_t id,
			texture* tex,
			const vec2& size,
			const std::array<vec2, 4>& texture_coords
		)
		{
			new(bundle + id) sprite2d{ tex, size, {-size.x / 2, -size.y / 2}, texture_coords };

			return *this;
		}

		inline void set_current(size_t id) noexcept
		{
			current = id;
		}

		inline sprite2d& get_current() noexcept
		{
			return bundle[current];
		}

		sprite2d* bundle;
		size_t size;
		size_t current;
	};

	struct tile_map2d
	{
		template<size_t width, size_t height>
		tile_map2d(const texture& tex, const vec2& tile_input_size, const vec2& tile_output_size, const vec2(&indexses)[width][height])
			: tex_source(tex),
			tile_block_size(tile_input_size),
			tile_texture_coords(width * height),
			map_width(width),
			map_height(height),
			tile_screen_size(tile_output_size)
		{
			for (size_t i = 0; i < height; i++)
			{
				for (size_t j = 0; j < width; j++)
				{
					tile_texture_coords[j + (i * width)] = tex_source.get_sub_texture_coords(tile_input_size, indexses[j][i]);
				}
			}
		};

		~tile_map2d()
		{

		}

		void set(size_t x, size_t y, const vec2& new_tile_pos)
		{
			tile_texture_coords[x + (y * map_width)] = tex_source.get_sub_texture_coords(tile_block_size, new_tile_pos);
		}

		const texture& tex_source;
		size_t map_width;
		size_t map_height;

		vec2 tile_screen_size;

		vec2 tile_block_size;

		std::vector<std::array<vec2, 4>> tile_texture_coords;
	};

	template<size_t frame_count>
	struct anim_sprite2d_params
	{
		const vec2& frame_input_size;
		const vec2& frame_output_size;
		const vec2(&frames)[frame_count];
		float frame_step_time;
		uint32_t frame_begin;
	};

	struct anim_sprite2d
	{
		template<size_t frame_count>
		anim_sprite2d(texture& source, const vec2& frame_input_size, const vec2& frame_output_size, const vec2(&frames)[frame_count], float frame_step_time, uint32_t frame_begin)
			:	tex_source(source),
				frame_block_size(frame_input_size),
				frame_screen_size(frame_output_size),
				frames_texture_coords(new std::array<vec2, 4>[frame_count]),
				frame_time(frame_step_time)
		{
			this->frame_ammout = frame_count;
			frame_current = frame_begin;

			for (size_t i = 0; i < frame_ammout; i++)
			{
				new(frames_texture_coords + i) std::array<vec2, 4>(tex_source.get_sub_texture_coords(frame_input_size, frames[i]));
			}
		}

		template<size_t frame_count>
		anim_sprite2d(texture& source, const anim_sprite2d_params<frame_count>& params)
			: tex_source(source),
			frame_block_size(params.frame_input_size),
			frame_screen_size(params.frame_output_size),
			frames_texture_coords(new std::array<vec2, 4>[frame_count]),
			frame_time(params.frame_step_time)
		{
			this->frame_ammout = frame_count;
			frame_current = params.frame_begin;

			for (size_t i = 0; i < frame_ammout; i++)
			{
				new(frames_texture_coords + i) std::array<vec2, 4>(tex_source.get_sub_texture_coords(params.frame_input_size, params.frames[i]));
			}
		}

		~anim_sprite2d()
		{
			delete[] frames_texture_coords;
			frames_texture_coords = nullptr;
		}

		void set(size_t frame, const vec2& new_frame)
		{
			frames_texture_coords[frame] = tex_source.get_sub_texture_coords(frame_block_size, new_frame);
		}

		void next()
		{		
			frame_current++;
			frame_current %= frame_ammout;
		}

		bool is_done()
		{
			return frame_current == (frame_ammout - 1);
		}

		texture& tex_source;
		size_t frame_ammout;
		size_t frame_current;

		vec2 frame_block_size;
		vec2 frame_screen_size;

		std::array<vec2, 4>* frames_texture_coords;

		float frame_time;
	};


	BETTER_ENUM(entity_tag, uint32_t, NONE, BUTTON, PLAYER, BLOCK, PAD, COIN, COIN_COUNTER, FPS_COUNTER);

	struct tag_component
	{
		entity_tag value;

		friend std::ostream& operator<<(std::ostream& os, tag_component& self)
		{
			return os << std::to_string(self.value);
		}
	};


	struct anim_sprite2d_bundle
	{
		anim_sprite2d_bundle(size_t size)
		{
			bundle = (anim_sprite2d*)std::calloc(size, sizeof(anim_sprite2d));
			//bundle = new anim_sprite2d[size];
			current_anim = 0;
			bundle_size = size;
		}

		template<typename...Args>
		anim_sprite2d_bundle& put(size_t idx, Args&&...args)
		{
			if (!(idx < bundle_size))
			{
				KW_LOG_CRITICAL_ERROR("out of bounds animation bundle access");
				return *this;
			}

			bundle[idx].~anim_sprite2d();
			new(bundle + idx) anim_sprite2d(std::forward<Args>(args)...);
			return *this;
		}

		anim_sprite2d& current()
		{
			return bundle[current_anim];
		}

		void next()
		{
			bundle[current_anim].next();
		}

		bool is_done()
		{
			return bundle[current_anim].is_done();
		}

		anim_sprite2d* bundle;
		size_t current_anim;
		size_t bundle_size;
		float time;
	};

	struct script_component
	{
		~script_component()
		{
			if (_script_body)
			{
				delete _script_body;
				_script_body = nullptr;
			}
		}

		struct script_base
		{
			script_base(entity& e)
				: self(e)
				, context(e.get_scene())
			{}

			virtual ~script_base() {}

			scene& context;
			entity& self;

			virtual void on_create() = 0;
			virtual void on_update(float delta_time) = 0;

			virtual uint64_t get_hash() { return typeid(*this).hash_code(); };
		};

		template<typename ScriptBody>
		void bind(entity& self) noexcept
		{
			_script_body = new ScriptBody(self);
		}

		void on_create() noexcept
		{
			_script_body->on_create();
		}

		void on_update(float delta_time) noexcept
		{
			_script_body->on_update(delta_time);
		}

		template<typename ScriptBody>
		inline ScriptBody& get_body() noexcept
		{
			return *static_cast<ScriptBody*>(_script_body);
		}

		script_base* _script_body;
	};

	struct camera_component
	{
		glm::mat4 proj = glm::mat4(1.0f);
		glm::mat4 view;
	};

	struct physics2d
	{
		vec2 force;
		vec2 acceleration;
		vec2 velocity;
		float mass = 1;
		float drag = 0.01;
	};

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct collider2d
	{
		vec2 size;
		vec2 offset;

		inline void make_centered() noexcept
		{
			offset = { -size.x / 2, -size.y / 2 };
		}

		static inline bool is_point_inside(const vec2& point, const transform& tr_a, const collider2d& a)
		{
			return	tr_a.position.x + a.offset.x + a.size.x > point.x &&
					tr_a.position.x + a.offset.x			< point.x &&
					tr_a.position.y + a.offset.y + a.size.y > point.y &&
					tr_a.position.y + a.offset.y			< point.y;
		}

		static inline bool is_colliding(const transform& tr_a, const transform& tr_b, const collider2d& a, const collider2d& b)
		{
			return	tr_a.position.x + a.offset.x + a.size.x > tr_b.position.x + b.offset.x				&&
					tr_a.position.x + a.offset.x			< tr_b.position.x + b.offset.x + b.size.x	&&
					tr_a.position.y + a.offset.y + a.size.y > tr_b.position.y + b.offset.y				&&
					tr_a.position.y + a.offset.y			< tr_b.position.y + b.offset.y + b.size.y;
		}
	};

	struct UUID
	{
		uint64_t id;

		inline bool operator !=(const UUID& other)
		{
			return id != other.id;
		}

		UUID()
		{
			((uint32_t*)&id)[0] = rand();
			((uint32_t*)&id)[1] = rand();
		}
	};
}

struct callback_component
{
	~callback_component()
	{
		delete _callback_body;
		_callback = nullptr;
	}

	template<typename Fn>
	void bind(Fn callback)
	{
		_callback_body = new Fn(std::forward<Fn>(callback));
		_callback = [](void* fn)
			{
				(*static_cast<Fn*>(fn))();
			};
	}

	void invoke()
	{
		if(_callback)
		{
			_callback(_callback_body);
		}
	}
		
public:
	void(*_callback)(void*);
	void* _callback_body;

};