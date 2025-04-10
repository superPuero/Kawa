#pragma once
#pragma once
#include "../../kawa.h"


namespace kawa
{
	BETTER_ENUM(play_button_frames, uint32_t, normal, highlighted, pressed);

	struct play_button_script : script_component::script_base
	{
		using script_base::script_base;

		transform& tr = self.get<transform>();
		collider2d& cld = self.get<collider2d>();
		sprite2d_bundle& spb = self.get<sprite2d_bundle>();
		callback_component& cb = self.get<callback_component>();

		const float transition_time = 0.1f;
		float curr_transition_time = 0;

		bool is_pressed = false;

		float hover_time = 0;

		bool is_hovered()
		{
			vec2 mouse_pos;

			application::application_instance->_window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			return collider2d::is_point_inside(mouse_pos, tr, cld);
		}

		void on_hover(float delta_time)
		{
			if (hover_time >= 0.2)
			{
				tr.scale.x = 1.2;
				tr.scale.y = 1.2;

			}
			else
			{
				hover_time += delta_time * 2;

				tr.scale.x += delta_time * 2;
				tr.scale.y += delta_time * 2;
			}
		}


		void on_non_hover(float delta_time)
		{
			if (hover_time <= 0)
			{
				tr.scale.x = 1.0;
				tr.scale.y = 1.0;
			}
			else
			{
				hover_time -= delta_time * 2;

				tr.scale.x -= delta_time * 2;
				tr.scale.y -= delta_time * 2;
			}
		}

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
			if (is_pressed)
			{
				spb.current = pressed;

				if (curr_transition_time >= transition_time)
				{
					cb.invoke();
					is_pressed = false;
					curr_transition_time = 0;
				}
				else
				{
					curr_transition_time += delta_time;
				}
			}
			else if (is_hovered())
			{
				on_hover(delta_time);
				spb.current = highlighted;
				if (application::application_instance->_window.is_mouse_button_press(GLFW_MOUSE_BUTTON_1))
				{
					is_pressed = true;
				}
			}
			else
			{
				on_non_hover(delta_time);
				spb.current = normal;
			}
		}
	};

	inline void play_button_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>();
		self.emplace<tag_component>(BUTTON);
		self.emplace<transform>(vec3{ application::application_instance->_window.win_width / 2 , 600.0f , -1});
		self.emplace<collider2d>(vec2{ 716,  160 }).make_centered();

		self.emplace<callback_component>().bind([]() {application::application_instance->set_scene(1); std::cout << "here"; });

		self.emplace<sprite2d_bundle>(3)
			.put_centered(normal, &renderer::textures["play_normal"], vec2{ 716.0f, 160.0f }, renderer::textures["play_normal"].get_texture_coords())
			.put_centered(highlighted, &renderer::textures["play_highlighted"], vec2{ 716.0f, 160.0f }, renderer::textures["play_highlighted"].get_texture_coords())
			.put_centered(pressed, &renderer::textures["play_pressed"], vec2{ 716.0f, 160.0f },  renderer::textures["play_pressed"].get_texture_coords());

		self.emplace<script_component>().bind<play_button_script>(scene, self);
	}
}