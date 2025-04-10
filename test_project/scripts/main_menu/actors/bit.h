#pragma once
#include "../../kawa.h"


namespace kawa
{
	struct bit_script : script_component::script_base
	{
		using script_base::script_base;

		transform& tr = self.get<transform>();

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
			application::application_instance->_window.get_mouse_pos(tr.position.x, tr.position.y);
		}
	};

	inline void bit_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>().id;

		self.emplace<tag_component>(NONE);

		self.emplace<transform>(vec3{ 200,200, -1});
		self.emplace<collider2d>(vec2{ 200,  200 });

		self.emplace<script_component>().bind<bit_script>(scene, self);
	}
}