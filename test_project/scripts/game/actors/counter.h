#pragma once
#include "../../kawa.h"

namespace kawa
{
	struct counter_script : script_component::script_base
	{
		using script_base::script_base;

		text_component& tc = self.get<text_component>();
		uint32_t counter = 0;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			tc.data = std::to_string(counter);
		}
	};

	inline void counter_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<tag_component>(COIN_COUNTER);
		self.emplace<text_component>("", vec2{ 32, 64 });
		self.emplace<transform2d>(vec2{ 0, application::application_instance->_window.win_height - 128 });

		self.emplace<script_component>().bind<counter_script>(scene, self);
	}
	
}