#pragma once

#include "../../kawa.h"

#include "bullet.h"

namespace kawa
{
	KW_SCRIPT_DECL(ship_script)
	{		
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();
		physics2d& phys = self.get<physics2d>();
		//text_component& tc = self.get<text_component>();

		int v = 12;

		void on_create()
		{
		}

		void on_update(float delta_time)
		{
			vec2 mouse_pos;
			app::instance().window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			vec2 direction = glm::normalize(mouse_pos - vec2{ tr._position.x, tr._position.y});

			tr.set_rotation(vec3{ 0,0,std::atan2(direction.y, direction.x) - glm::pi<float>() / 2 });

			if (app::instance().window.is_key_down(GLFW_KEY_W))
			{
				phys.force.x = direction.x * 30;
				phys.force.y = direction.y * 30;
			}
			else
			{
				phys.force = { 0,0 };
			}

			//tc.data.clear();

			//tc.data += "x: " + std::to_string((int)tr._position.x) + '\n';
			//tc.data += "y: " + std::to_string((int)tr._position.y) + '\n';
			//tc.data += "vel x: " + std::to_string((int)phys.velocity.x) + '\n';
			//tc.data += "vel y: " + std::to_string((int)phys.velocity.y) + '\n';
			//tc.data += "fps: " + std::to_string((int)(1.0f / delta_time)) + '\n';

			if (app::instance().window.is_key_down(GLFW_KEY_3))
			{
				auto& bullet = context.entity_from_prefabs(bullet_prefab);
				bullet.get<transform>()._position = tr._position;
				bullet.get<transform>()._rotation = tr._rotation;
				auto& bullet_phys = bullet.get<physics2d>();
				bullet_phys.force = { direction.x * 12000, direction.y * 12000 };
			}
		}

		void _serealize(serealizer& ser)
		{
			ser._output << v << ' ';
		}

		void _deserealize(deserealizer& deser)
		{
			deser._input >> v;
		}
	};


	void ship_preafab(entity& self)
	{	
		self.emplace<transform>();

		self.emplace<sprite2d>
			(
				&renderer::textures["fighter_ship"],
				vec2{0.5,0.5 },
				vec2{},
				renderer::textures["fighter_ship"].get_texture_coords()
			).make_centred();

		self.emplace<collider2d>(vec2{ 32,32 }).make_centered();
		self.emplace<physics2d>();
		//self.emplace<text_component>("foo", vec2{0.2, 0.4});
		self.emplace<script_component>(&self).bind<ship_script>();

	}

}