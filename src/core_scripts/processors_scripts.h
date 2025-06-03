#pragma once

#include "../core/components/components.h"

namespace kawa
{
	namespace processors
	{
		inline void script_component_on_create(script_component& sc)
		{
			sc.on_create();
		}

		inline void script_component_on_update(float delta_time, script_component& sc)
		{
			sc.on_update(delta_time);
		}

		inline void physics_solver(float delta_time, transform& tr, physics2d& phys)
		{
			phys.acceleration = phys.force / phys.mass;

			phys.velocity += phys.acceleration * delta_time;

			phys.velocity *= (1.0f - phys.drag);

			tr.position.x += phys.velocity.x * delta_time;
			tr.position.y += phys.velocity.y * delta_time;

		}

		inline void button_component_update(float delta_time, transform& tr, collider2d& col, button_component& bc)
		{
			button_component::process(tr, col, bc);
			bc.update(delta_time);
		}

		inline void camera_processor(float delta_time, transform& tr, proj_camera_component& cc)
		{
			if(cc.primary)
			{ 
				cc.view = glm::inverse(tr.get_mat4());

				renderer::shader_map["test"].uniform_mat4("proj", (float*)&cc.proj);
				renderer::shader_map["test"].uniform_mat4("view", (float*)&cc.view);			
			}

		}
	}
}