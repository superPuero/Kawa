//#pragma once
//#include "../../kawa.h"
//
//#include "../shared/ortho_camera.h"
//
//#include "actors/play_button.h"
//#include "actors/home_button.h"
//#include "actors/bit.h"					
//
//namespace main_menu
//{ 
//	KW_SCENE_LOGIC_DECL(main_menu_script)
//	{
//		KW_SCENE_LOGIC_HEADER();
//
//		void on_create()
//		{
//			self.query(processors::script_component_on_create);
//			KW_LOG(get_hash());
//		}
//
//		void on_update(float delta_time)
//		{
//			self.query(processors::script_component_on_update, delta_time);
//			self.query(processors::button_component_update, delta_time);
//
//			if (app::instance().window.is_key_down(GLFW_KEY_X))
//			{
//				self.serealize("main_menu.kwscn");
//			}
//
//			self.cleanup();
//		}
//
//		void on_render()
//		{
//			self.query(renderer_scripts::default_sprite_bundle_render);
//			//self.query(renderer_scripts::default_true_collider_render);
//			self.query(renderer_scripts::default_transform_marker_render);
//			self.query(renderer_scripts::default_text_render);
//		}
//	};
//
//
//	inline void _assets()
//	{
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Play.png", "play_normal");
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Play.png", "play_highlighted");
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Play.png", "play_pressed");
//
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Normal/Home.png", "home_normal");
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Highlighted/Home.png", "home_highlighted");
//		renderer::load_texture("assets/Modern UI Pack/Buttons/Pressed 1/Home.png", "home_pressed");
//	}
//
//	inline void prefab(scene& self)
//	{
//		_assets();
//
//		self.entity_from_prefabs(play_button_prefab);
//		self.entity_from_prefabs(button_prototype, home_button_prefab);
//		self.entity_from_prefabs(bit_prefab);
//		self.entity_from_prefabs(ortho_camera_prefab);
//
//		self.bind<main_menu_script>();
//	}
//}
