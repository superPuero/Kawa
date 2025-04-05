#pragma once

#include <vector>

#include "glm.hpp"
#include "macros.h"

#include "../utils/utils.h"
#include "../renderer/texture.h"


namespace kawa
{
	class scene;
	class entity;

	struct cube
	{
		vec3 size;
	};

	struct text_component
	{
		std::string data;
		vec2 size;
	};

	struct transform2d
	{
		vec2 position;
	};

	struct sprite2d
	{
		texture* tex;
		std::array<vec2, 4> texture_coords;
		vec2 size;
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
			const std::array<vec2, 4>& texture_coords,
			const vec2& size)
		{
			new(bundle + id) sprite2d{ tex, texture_coords, size };

			return *this;
		}

		inline void set_current(size_t id) noexcept
		{
			current = id;
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
			//delete[] tile_texture_coords;
			//tile_texture_coords = nullptr;


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

		//std::array<vec2, 4>* tile_texture_coords;
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
				std::cout << "out of bounds animation bundle access" << '\n';
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

	//BETTER_ENUM(script_tag, uint32_t, none, hume_button, exit_button, camera, bit);

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
			script_base(scene& s, entity& e)
				:	context(s),
					self(e)
			{}

			virtual ~script_base() {}

			scene& context;
			entity& self;

			virtual void on_create() = 0;
			virtual void on_update(float delta_time) = 0;

			virtual uint64_t get_hash() { return typeid(*this).hash_code(); };
		};

		template<typename ScriptBody>
		void bind(scene& s, entity& e) noexcept
		{
			_script_body = new ScriptBody( s, e );
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

	struct transform3d
	{
		transform3d() = default;

		transform3d(vec3&& vec)
		{
			position = vec;
		}


		transform3d(vec3&& vec, vec3&& vec2)
		{
			position = vec;
			rotation = vec2;
		}

		vec3 position;
		vec3 rotation;
		vec3 front;
	};

	struct physics2d
	{
		vec2 velocity;
		vec2 force;
		float mass;
		float gravity;
		bool on_ground;
	};

	struct physics3d
	{
		vec3 velocity;
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

		static inline bool is_point_inside(const vec2& point, const transform2d& tr_a, const collider2d& a)
		{
			return	tr_a.position.x + a.offset.x + a.size.x > point.x &&
					tr_a.position.x + a.offset.x			< point.x &&
					tr_a.position.y + a.offset.y + a.size.y > point.y &&
					tr_a.position.y + a.offset.y			< point.y;
		}

		static inline bool is_colliding(const transform2d& tr_a, const transform2d& tr_b, const collider2d& a, const collider2d& b)
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

		~UUID()
		{
			std::cout << "deleted: " << id << '\n';
		}
	};
}