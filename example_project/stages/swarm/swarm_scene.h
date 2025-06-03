#pragma once

#include "../../kawa.h"

#include "../shared/ortho_camera.h"
#include "../shared/fps_counter.h"

#include "actors/unit.h"

namespace swarm
{
	using namespace kawa;
	KW_SCENE_LOGIC_DECL(swarm_scene_logic)
	{
		KW_SCENE_LOGIC_HEADER();

		void on_create()
		{
			self.query(processors::script_component_on_create);
		}

		void on_update(float delta_time)
		{
			self.query(processors::script_component_on_update, delta_time);					
			self.query(processors::physics_solver, delta_time);
			
			self.query
			(
				[&](transform& tr, UUID& uuid, physics2d& ph)
				{
					if (uuid.id == 116307714398273)
					{
						vec2 mouse_pos{};

						//app::instance().window.get_mouse_pos(mouse_pos.x, mouse_pos.y);
						tr.position.x = mouse_pos.x;
						tr.position.y = mouse_pos.y;
					}
				}
			);

			self.query
			(
				[&](transform& tr, UUID& uuid, physics2d& ph)
				{
					self.query
					(
						[&](transform& tr_, UUID& uuid_, physics2d& ph_)
						{
							if (uuid != uuid_)
							{
								float dx = tr.position.x - tr_.position.x;
								float dy = tr.position.y - tr_.position.y;

								if (dx * dx + dy * dy <= 3000)
								{
									renderer::draw_line(tr, tr_, 1, { 0, 0, 0, 0 });
								}
							}
						}					
					);
				}
			);
			
			if (app::instance().window.is_key_down(GLFW_KEY_J))
			{
				self.serealize("swarm.kwscn");
			}

			self.cleanup();
		}

		void on_render()
		{
			self.query(renderer_scripts::default_sprite_render);
			self.query(renderer_scripts::default_true_collider_render);
			self.query(renderer_scripts::default_text_render);

		}
	};


	inline void _assets()
	{

	}

	inline void prefab(scene& self)
	{
		self.entity_from_prefabs(ortho_camera_prefab);
		self.entity_from_prefabs(fps_counter_prefab);

		for (size_t i = 0; i < 800; i++)
		{											  
			self.entity_from_prefabs(unit_prefab);
		}
		
		self.bind<swarm_scene_logic>();

		//self.deserealize("swarm.kwscn");
	}
}