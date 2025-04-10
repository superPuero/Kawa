#pragma once

#include "../../kawa.h"

namespace kawa
{
	struct ship_script : script_component::script_base
	{		
		using script_base::script_base;

		transform& tr = self.get<transform>();
		physics2d& phys = self.get<physics2d>();


		void on_create()
		{

		}

		void on_update(float delta_time) 
		{
			vec2 mouse_pos;
			application::application_instance->_window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			vec2 direction = glm::normalize(mouse_pos - vec2{ tr.position });

			tr.rotation.z = std::atan2(direction.y, direction.x) - glm::pi<float>()/2;


			if (application::application_instance->_window.is_key_down(GLFW_KEY_W))
			{
				phys.force.x = direction.x * 3000;
				phys.force.y = direction.y * 3000;
			}
			else
			{
				phys.force = { 0,0 };
			}
		}		  
	};

	void ship_preafab(entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>
		(
			vec3
			{
				application::application_instance->_window.win_width / 2,
				application::application_instance->_window.win_height / 2,
				-1
			}
		);

		self.emplace<sprite2d>
			(
				&renderer::textures["fighter_ship"],
				vec2{128,128},
				vec2{},
				renderer::textures["fighter_ship"].get_texture_coords()
			).make_centred();

		self.emplace<physics2d>();

		self.emplace<script_component>().bind<ship_script>(self);

	}

}