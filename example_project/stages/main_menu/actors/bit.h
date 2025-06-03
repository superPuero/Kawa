#pragma once
#include "../../kawa.h"


namespace kawa
{
	KW_SCRIPT_DECL(bit_script)
	{
		KW_SCRIPT_HEADER();
		
		transform& tr = self.get<transform>();

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
			vec3 mouse_pos{ 0,0,-1 };

			app::instance().window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			tr.set_position(mouse_pos);
		}
	};

	inline void bit_prefab(entity& self)
	{
		self.emplace<UUID>().id;

		self.emplace<tag_component>(NONE);

		self.emplace<transform>(vec3{ 200,200, -1});
		self.emplace<collider2d>(vec2{ 200,  200 });

		self.emplace<script_component>(&self).bind<bit_script>();
	}
}