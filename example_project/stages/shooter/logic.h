//#pragma once
//
//#include "../../kawa.h"
//  
//#include "actors/background.h"
//#include "actors/ship.h"
//#include "actors/asteroid.h"
//#include "../shared/ortho_camera.h"
//#include "../shared/perspective_camera.h"
//
//
//namespace shooter
//{
//	using namespace kawa;
//	KW_SCENE_LOGIC_DECL(shooter_script)
//	{
//		KW_SCENE_LOGIC_HEADER();
//
//		void on_create()
//		{
//			self.query(processors::script_component_on_create);
//		}
//
//		void on_update(float delta_time)
//		{
//			self.query(processors::script_component_on_update, delta_time);
//			self.query(processors::physics_solver, delta_time);
//
//			if (app::instance().window.is_key_down(GLFW_KEY_X))
//			{
//				self.serealize("shooter.kwscn");
//			}
//
//			self.cleanup();
//		}
//
//		void on_render()
//		{
//			self.query(renderer_scripts::default_sprite_render);
//			//self.query(renderer_scripts::default_transform_marker_render);
//			//self.query(renderer_scripts::default_text_render);
//			//self.query(renderer_scripts::default_true_collider_render);
//		}
//	};
//
//	inline void _assets()
//	{
//		renderer::load_texture("assets/Foozle_2DS0012_Void_FleetPack_1/Foozle_2DS0012_Void_EnemyFleet_1/Kla'ed/Base/PNGs/Kla'ed - Fighter - Base.png", "fighter_ship");
//		renderer::load_texture("assets/Foozle_2DS0015_Void_EnvironmentPack/Foozle_2DS0015_Void_EnvironmentPack/Backgrounds/PNGs/Condesed/Starry background  - Layer 02 - Stars.png", "stars_background");
//		renderer::load_texture("assets/Foozle_2DS0015_Void_EnvironmentPack/Foozle_2DS0015_Void_EnvironmentPack/Asteroids/PNGs/Asteroid 01 - Base.png", "asteroid");
//		renderer::load_texture("assets/Foozle_2DS0015_Void_EnvironmentPack/Foozle_2DS0015_Void_EnvironmentPack/Asteroids/PNGs/Asteroid 01 - Base.png", "bullet");
//	}
//
//	inline void prefab(scene& self)
//	{
//		_assets();
//
//		self.entity_from_prefabs(perspective_camera_prefab);
//		self.entity_from_prefabs(background_prefab);
//		self.entity_from_prefabs(ship_preafab);	
//		for (size_t i : std::array<size_t, 10>{})
//		{
//			self.entity_from_prefabs(asteroid_prefab);
//		}
//
//		//self.deserealize("shooter.kwscn");
//
//		self.bind<shooter_script>();
//	}
//}