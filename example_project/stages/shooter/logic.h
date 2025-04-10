#pragma once

#include "../../kawa.h"
  
#include "actors/background.h"
#include "actors/ship.h"
#include "actors/asteroid.h"
#include "../shared/ortho_camera.h"

namespace shooter
{
	using namespace kawa;
	struct script : scene_logic_body
	{
		void on_create()
		{
			self.query(processors::script_component_on_create);
		}

		void on_update(float delta_time)
		{
			self.query(processors::script_component_on_update, delta_time);
			self.query(processors::physics_solver, delta_time);
		}

		void on_render()
		{
			self.query(renderer_scripts::default_sprite_render);
			//self.query(renderer_scripts::default_transform_marker_render);
		}
	};

	inline void _assets()
	{
		renderer::load_texture("assets/Foozle_2DS0012_Void_FleetPack_1/Foozle_2DS0012_Void_EnemyFleet_1/Kla'ed/Base/PNGs/Kla'ed - Fighter - Base.png", "fighter_ship");
		renderer::load_texture("assets/Foozle_2DS0015_Void_EnvironmentPack/Foozle_2DS0015_Void_EnvironmentPack/Backgrounds/PNGs/Condesed/Starry background  - Layer 02 - Stars.png", "stars_background");
		renderer::load_texture("assets/Foozle_2DS0015_Void_EnvironmentPack/Foozle_2DS0015_Void_EnvironmentPack/Asteroids/PNGs/Asteroid 01 - Base.png", "asteroid");
	}

	inline void prefab(scene& self)
	{
		_assets();

		self.entity_from_prefab(ortho_camera_prefab);

		self.entity_from_prefab(background_prefab);
		self.entity_from_prefab(ship_preafab);

		self.entity_from_prefab(asteroid_prefab);
		self.entity_from_prefab(asteroid_prefab);
		self.entity_from_prefab(asteroid_prefab);
		self.entity_from_prefab(asteroid_prefab);


		self.bind<script>();
	}
}