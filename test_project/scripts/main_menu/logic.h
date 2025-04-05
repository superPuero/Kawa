#pragma once
#include "../../kawa.h"

#include "actors.h"

namespace main_menu
{
	using namespace kawa;

	struct script : scene_logic_body
	{
		void on_create()
		{
			self.query([](script_component& sc) { sc.on_create(); });
		}

		void on_update(float delta_time)
		{
			self.query([](float delta_time, script_component& sc) { sc.on_update(delta_time); }, delta_time);
			self.cleanup();
		}

		void on_render()
		{
			//self.query(renderer2d_scripts::default_collider_render);
			self.query(renderer2d_scripts::default_sprite_render);
			self.query(renderer2d_scripts::default_sprite_bundle_render);

			if (application::application_instance->_window.is_key_down(GLFW_KEY_H))
			{
				self.serealize("menu.scene");
			}
			//self.query(renderer2d_scripts::default_tile_map_render);
			//self.query(renderer2d_scripts::default_anim_sprite_render);
			//self.query(renderer2d_scripts::default_anim_sprite_bundle_render);
			//self.query(renderer2d_scripts::default_text_render);
		}
	};							   


	static void _assets()
	{
		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Play.png", "play_normal");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Play.png", "play_highlighted");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Play.png", "play_pressed");

		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Home.png", "home_normal");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Home.png", "home_highlighted");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Home.png", "home_pressed");
	}

	static void prefab(scene& self)
	{
		_assets();

		self.entity_from_prefab(play_button_prefab::prefab);
		self.entity_from_prefab(home_button_prefab::prefab);
		self.entity_from_prefab(bit_prefab::prefab);
		self.entity_from_prefab(ortho_camera_prefab::prefab);

		self.bind<script>();

		self.on_create();
	}
};

