#pragma once

#include "../../kawa.h"

namespace kawa
{
	struct block2_script :script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{

		};

		void on_update(float delta_time)
		{

		}
	};

	inline void block2_prefab(scene& scene, entity& self)
	{
		vec2 idxs[10][1] = { {vec2{0, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{3, 19}} };
		self.emplace<UUID>().id = self.id();
		self.emplace<tag_component>(BLOCK);
		self.emplace<transform2d>(vec2{ 100.0f, 100.0f });
		self.emplace<collider2d>(vec2{ 1200,  25 }, vec2{ 0 , 0 });

		self.emplace<tile_map2d>(renderer::textures["tiles"], glm::vec2{ 20.0f, 20.0f }, glm::vec2{ 40.0f, 40.0f }, idxs);

		self.emplace<script_component>().bind<block2_script>(scene, self);
	}
}