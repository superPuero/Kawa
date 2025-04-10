#pragma once
#include "../../kawa.h"

namespace kawa
{
	struct text_script : script_component::script_base
	{
		using script_base::script_base;

		text_component& tc = self.get<text_component>();
		float timer = 0;


		void on_create()
		{
			tc.data = "fps";
		}

		void on_update(float delta_time)
		{
			if (timer > 0.5)
			{
				timer = 0;
				tc.data = std::to_string((int)(1.0f / delta_time));
			}
			else
			{
				timer += delta_time;
			}
		}
	};

	inline void text_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<transform2d>(vec2{ 0, application::application_instance->_window.win_height - 64 });
		self.emplace<text_component>("", vec2{ 32, 64 });
		self.emplace<script_component>().bind<text_script>(scene, self);
	}
	

}