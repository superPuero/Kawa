#pragma once

#include "../../kawa.h"

namespace kawa
{
	//struct bullet_script : script_component::script_base< bullet_script>
	KW_SCRIPT_DECL(bullet_script)
	{
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			if (tr._position.x > 2000 || tr._position.x < 0 && tr._position.y > 200 || tr._position.y < 0)
			{
				context.fetch_remove(self.id());
			}
		}

	};

	inline void bullet_prefab(entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>();
		self.emplace<physics2d>();
		self.emplace<collider2d>(vec2{16,16}).make_centered();

		self.emplace<sprite2d>
			(
				&renderer::textures["bullet"],
				vec2{ 16,16 },
				vec2{},
				renderer::textures["bullet"].get_texture_coords()
			);

		self.emplace<script_component>(&self).bind<bullet_script>();

	}
}