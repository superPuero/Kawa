#pragma once
#include "../../kawa.h"

namespace kawa
{
	struct asteroid_script : script_component::script_base
	{
		using script_base::script_base;

		bool is_dead = false;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			
		}
	};

	void asteroid_prefab(entity& self)
	{
		self.emplace<UUID>();
		auto& tr = self.emplace<transform>
			(
				vec3{rand()%application::window_instange->win_width, rand() % application::window_instange->win_height, -1}	,
				vec3{ 0, 0, cos(rand())}
			);
		 
		self.emplace<physics2d>();

		self.emplace<sprite2d>
			(
				&renderer::textures["asteroid"],
				vec2{ 96 * 2, 96 * 2},
				vec2{},
				renderer::textures["asteroid"].get_texture_coords()
			).make_centred();

		self.emplace<script_component>().bind<asteroid_script>(self);


	}
}