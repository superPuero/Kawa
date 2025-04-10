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
		void on_create()
		{
			self.query(processors::script_component_on_create);
		}

		void on_update(float delta_time)
		{
			self.query(processors::script_component_on_update, delta_time);
			self.cleanup();
		}

		void on_render()
		{
			self.query(renderer_scripts::default_sprite_bundle_render);
			
			//self.query(renderer_scripts::default_true_collider_render);

			//self.query(renderer_scripts::default_transform_marker_render);
		}
	};


	inline void _assets()
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
		_assets();

		self.entity_from_prefab(play_button_prefab);
		self.entity_from_prefab(home_button_prefab);
		self.entity_from_prefab(bit_prefab);
		self.entity_from_prefab(ortho_camera_prefab);

		self.bind<main_menu_script>();
	}
}
