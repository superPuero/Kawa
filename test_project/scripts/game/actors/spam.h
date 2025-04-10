#pragma once

#include "../../kawa.h"

namespace kawa
{
	struct spam_script : script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{

		}
	};


	inline void spam_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<transform2d>(vec2(std::rand() % application::application_instance->_window.win_width, std::rand() % application::application_instance->_window.win_height));
		self.emplace<collider2d>(vec2{ 16,16 }, vec2{ 0,0 });

		self.emplace<script_component>().bind<spam_script>(scene, self);

	}
}