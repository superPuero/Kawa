#pragma once

#include <ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "../../utils/utils.h"
#include "../../renderer/texture.h"
#include "../../core/data_structures/dyn_array.h"
#include "../../debug/debug.h"
#include "../../scene/entity.h"
#include "../app.h"
#include "../macros.h"

#include "script_component.h"
namespace kawa
{
	class serealizer;
	class deserealizer;	

	struct transform
	{
		transform(vec3 pos = vec3(0,0,0), vec3 rot = vec3(0,0,0), vec3 slc = vec3(1, 1, 1))
		{
			position = pos;
			rotation = rot;
			scale = slc;
		}

		vec3 position = { 0,0,0 };
		vec3 rotation = { 0,0,0 };
		vec3 scale = { 1.0f, 1.0f, 1.0f };

	public:
		inline glm::mat4& get_mat4() const noexcept
		{
			_mat = glm::mat4(1.0f);

			_mat = glm::translate(_mat, position);

			_mat *= glm::toMat4(glm::quat(rotation));

			_mat = glm::scale(_mat, scale);

			return _mat;
		}

	private:
		mutable glm::mat4 _mat;
	};

	struct text_component
	{
		vec2 size	= {16 , 32 };
		vec2 offset = {0,0};
		std::string data;
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
		~sprite2d_bundle() = default;

		inline sprite2d_bundle& add
		(
			texture* tex,
			const vec2& size,
			const vec2& offset,
			const std::array<vec2, 4>& texture_coords
		)
		{
			bundle.emplace_back(tex, size, offset, texture_coords);
			return *this;
		}

		inline sprite2d_bundle& add_centered
		(
			texture* tex,
			const vec2& size,
			const std::array<vec2, 4>& texture_coords
		)
		{
			bundle.emplace_back(tex, size, vec2{ -size.x / 2, -size.y / 2 }, texture_coords);
			return *this;
		}

		inline sprite2d_bundle& set_current(size_t id) noexcept
		{
			current = id;
			return *this;
		}

		inline sprite2d& get_current() noexcept
		{
			return bundle[current];
		}

		dyn_array<sprite2d> bundle;
		size_t size;
		size_t current;
	};

	struct anim_sprite2d
	{
		
	};

	struct proj_camera_component
	{
		glm::mat4 proj = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		float fov = 90.0f;
		bool primary = false;

	};

	struct ortho_camera_component
	{
		glm::mat4 proj;
		glm::mat4 view;

	};

	struct physics2d
	{
		vec2 force;
		vec2 acceleration;
		vec2 velocity;
		float mass = 1;
		float drag = 0;
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
					tr_a.position.x + a.offset.x			 < tr_b.position.x + b.offset.x + b.size.x	&&
					tr_a.position.y + a.offset.y + a.size.y > tr_b.position.y + b.offset.y				&&
					tr_a.position.y + a.offset.y			 < tr_b.position.y + b.offset.y + b.size.y;
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

	struct button_component
	{
		bool is_pressed;	
		bool is_hovered;

		inline void reset()
		{
			is_pressed = false;
			is_hovered = false;
		}

		struct button_logic_common 
		{
		public:
			virtual uint64_t get_hash() { return typeid(*this).hash_code(); };
		};

		template<typename ButtonLogicBody>
		struct button_logic_base : button_logic_common, bindable_body_base<button_component, ButtonLogicBody>
		{
			button_logic_base(entity& e, bool& p, bool& h)
				: self(e)
				, context(e.get_scene())
				, is_pressed(p)
				, is_hovered(h)
			{}

			virtual ~button_logic_base() {}

			scene& context;
			entity& self;

			bool& is_hovered;
			bool& is_pressed;

			void on_press(float delta_time) { KW_LOG_WARNING("Executing on_press methond on button logic body where its not implemented");};
			void on_hover(float delta_time) { KW_LOG_WARNING("Executing on_hover methond on button logic body where its not implemented");};
			void on_non_hover(float delta_time) { KW_LOG_WARNING("Executing on_non_hover methond on button logic body where its not implemented"); }		

		};


		template<typename ButtonLogicBody>
		void bind(void* e = nullptr)
		{
			_button_body = new ButtonLogicBody(*static_cast<entity*>(e), is_pressed, is_hovered);

			_process_func = [](void* obj, bool is_pressed, bool is_hovered, float delta_time)
				{
					auto button_body = static_cast<ButtonLogicBody*>(obj);
					if (is_pressed)
					{
						button_body->on_press(delta_time);
					}
					else if (is_hovered)
					{
						button_body->on_hover(delta_time);
					}
					else
					{
						button_body->on_non_hover(delta_time);
					}
				};
		}

		void update(float delta_time)
		{
			if (_process_func)
			{
				_process_func(_button_body, is_pressed, is_hovered, delta_time);
			}
		}	
		
		button_logic_common* _button_body;

		void(*_process_func)(void*, bool, bool, float);

		static inline void process(transform& tr, collider2d& col, button_component& bc)
		{
			vec2 mouse_pos;

			//app::instance().window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			bc.is_hovered = collider2d::is_point_inside(mouse_pos, tr, col);

			if (bc.is_hovered && app::instance().window.is_mouse_button_press(GLFW_MOUSE_BUTTON_1))
			{
				bc.is_pressed = true;
			}
		}	
	};
}

#define KW_BUTTON_LOGIC_HEAD(logic_name) logic_name : button_component::button_logic_base<logic_name>




