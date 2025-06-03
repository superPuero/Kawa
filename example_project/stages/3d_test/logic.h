#pragma once
#include "../../kawa.h"

#include "../shared/perspective_camera.h"
#include "actors/square.h"
#include "actors/player.h"


namespace test3d
{
	using namespace kawa;
	KW_SCENE_LOGIC_DECL(test3d_scene_logic)
	{
		KW_SCENE_LOGIC_HEADER();

		void on_create()
		{
			self.query(processors::script_component_on_create);
			renderer::load_mesh("iron_man", "assets/models/cube.obj");
		}

		void on_update(float delta_time)
		{
			renderer::draw_mesh("iron_man");

			self.query(processors::script_component_on_update, delta_time);
			self.query(processors::camera_processor, delta_time);
		}

		void on_render()
		{
			self.query(renderer_scripts::default_sprite_bundle_render);
			

			//self.query(renderer_scripts::default_transform_marker_render);
			//self.query(renderer_scripts::default_true_collider_render);

		}
	};


	inline void _assets()
	{

	}

	inline void prefab(scene& self)
	{
		_assets();

		self.entity_from_prefabs(test3d::square_prefab);
		self.entity_from_prefabs(test3d::player_prefab);

		self.bind<test3d_scene_logic>();
	}
}