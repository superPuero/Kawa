#pragma once
#pragma once
#include "../../kawa.h"


namespace main_menu
{
	using namespace kawa;

	BETTER_ENUM(play_button_frames, uint32_t, normal, highlighted, pressed);

	struct KW_BUTTON_LOGIC_HEAD(play_button_logic)
	{
		using button_logic_base::button_logic_base;

		transform& tr = self.get<transform>();
		sprite2d_bundle& spb = self.get<sprite2d_bundle>();

		const float transition_time = 0.1f;
		float curr_transition_time = 0;

		float hover_time = 0;

		void on_press(float delta_time)
		{
			spb.current = pressed;

			if (curr_transition_time >= transition_time)
			{
				app::instance().set_scene("game");

				is_pressed = false;
				curr_transition_time = 0;
			}
			else
			{
				curr_transition_time += delta_time;
			}
		}

		void on_hover(float delta_time)
		{
			spb.current = highlighted;

			if (hover_time >= 0.5f)
			{
				tr.set_scale(vec3{ 1.2f, 1.2f, 1.0f });
				tr.set_rotation(vec3{ 0.0f, 0.0f, 0.1f });
			}
			else
			{
				hover_time += delta_time;

				float t = hover_time / 0.5f;
				t *= t;
				t *= t;

				tr.set_scale(vec3{ 1.0f + 0.2f * t, 1.0f + 0.2f * t, 1.0f });
				tr.set_rotation(vec3{ 0.0f, 0.0f, 0.1f * t });
			}
		}


		void on_non_hover(float delta_time)
		{
			spb.current = normal;

			if (hover_time <= 0)
			{
				tr.set_scale(vec3{ 1, 1, 1 });
				tr.set_rotation(vec3{ 0, 0, 0});
			}
			else
			{
				hover_time -= delta_time;

				float t = hover_time / 0.5f;
				t *= t;
				t *= t;


				tr.set_scale(vec3{ 1.0f + 0.2f * t, 1.0f + 0.2f * t, 1.0f });
				tr.set_rotation(vec3{ 0.0f, 0.0f, 0.1f * t });
			}
		}

	};

	inline void play_button_prefab(entity& self)
	{
		self.emplace<UUID>();

		self.emplace<transform>(vec3{ app::instance().window.win_width / 2, 600, 0 });

		self.emplace<collider2d>().size = vec2{ 716, 160 };
		self.emplace<collider2d>().make_centered();

		self.emplace<sprite2d_bundle>()
			.add_centered(&renderer::textures["play_normal"], vec2{ 716.0f, 160.0f }, renderer::textures["play_normal"].get_texture_coords())
			.add_centered(&renderer::textures["play_highlighted"], vec2{ 716.0f, 160.0f }, renderer::textures["play_highlighted"].get_texture_coords())
			.add_centered(&renderer::textures["play_pressed"], vec2{ 716.0f, 160.0f }, renderer::textures["play_pressed"].get_texture_coords());

		self.emplace<button_component>().bind<play_button_logic>(&self);
	}
}