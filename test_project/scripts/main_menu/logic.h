#pragma once
#include "../../kawa.h"

#include "../shared/ortho_camera.h"

#include "actors/play_button.h"
#include "actors/home_button.h"
#include "actors/bit.h"					

namespace main_menu
{ 
	struct main_menu_script : scene_logic_body
	{
		float val = 0;
		void on_create()
		{
			self.query([](script_component& sc) { sc.on_create(); });
		}

		void on_update(float delta_time)
		{
			self.query([](float delta_time, script_component& sc) { sc.on_update(delta_time); }, delta_time);
			self.cleanup();
			val += delta_time;
		}

		void on_render()
		{
			self.query(renderer2d_scripts::default_sprite_bundle_render);
			
			self.query(renderer2d_scripts::default_collider_render);

			//self.query(renderer2d_scripts::default_transform_marker_render);
		}
	};


	inline void _main_menu_assets()
	{
		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Play.png", "play_normal");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Play.png", "play_highlighted");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Play.png", "play_pressed");

		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Home.png", "home_normal");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Home.png", "home_highlighted");
		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Home.png", "home_pressed");
	}

	inline void prefab(kawa::scene& self)
	{
		_main_menu_assets();

		self.entity_from_prefab(play_button_prefab);
		self.entity_from_prefab(home_button_prefab);
		self.entity_from_prefab(bit_prefab);
		self.entity_from_prefab(ortho_camera_prefab);

		self.bind<main_menu_script>();

		self.on_create();
	}
}
