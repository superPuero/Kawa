#pragma once

#include "../../kawa.h"

namespace kawa
{
	KW_SCRIPT_DECL(unit_script)
	{
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();
		physics2d& ph = self.get<physics2d>();

		void on_create()
		{
			ph.drag = 0;

			float x = ((rand() % 100)-50)/50.0f;
			float y = ((rand() % 100) - 50) / 50.0f;

			float speed = 5;

			ph.velocity.x = x * speed;
			ph.velocity.y = y * speed;
		}

		void on_update(float delta_time)
		{
			if (tr.position.x <= 0 || tr.position.x >= app::instance().window.get_width())
			{
				ph.velocity.x *= -1;
			}
			if (tr.position.y <= 0 || tr.position.y >= app::instance().window.get_height())
			{
				ph.velocity.y *= -1;
			}
		}

		void _serealize(serealizer& ser)
		{
		}

		void _deserealize(deserealizer& deser)
		{

		}
	};


	void unit_prefab(entity& self)
	{
		self.emplace<UUID>();

		self.emplace<transform>(vec3{ rand() % app::instance().window.get_width(), rand() % app::instance().window.get_height(), -1});
		self.emplace<sprite2d>
			(
				&renderer::textures["white"],
				vec2{ 2,2 },
				vec2{},
				renderer::textures["white"].get_texture_coords()
			);

		self.emplace<collider2d>(vec2{ 2,2 }, vec2{0,0});
		self.emplace<physics2d>();

		self.emplace<script_component>(&self).bind<unit_script>();

	}

}