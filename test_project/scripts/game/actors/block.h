#pragma once
#include "../../kawa.h"

namespace kawa
{
	struct block_script : script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{

		};

		void on_update(float delta_time)
		{

		}
	};

	inline void block_prefab(scene& scene, entity& self)
	{
		vec2 idxs[1][1] = { {vec2{0, 15}} };
		self.emplace<UUID>().id = self.id();
		self.emplace<tag_component>(PAD);
		self.emplace<transform2d>(vec2{ 1000.0f, 200.0f });
		self.emplace<collider2d>(vec2{ 200,  200 }, vec2{ 0 , 0 });

		self.emplace<script_component>().bind<block_script>(scene, self);
	}	
}