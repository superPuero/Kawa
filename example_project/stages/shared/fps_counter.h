#pragma once
#include "../../kawa.h"

namespace kawa
{
	KW_SCRIPT_DECL(fps_counter_script)
	{
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();
		//text_component& tc = self.get<text_component>();

		const uint16_t frame_wait = 120;
		uint16_t frame_counter = 0;
		float accm = 0;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			if(frame_counter >= frame_wait)
			{ 
				//tc.data = std::to_string((int)(1.0f/(accm / frame_wait)));
				frame_counter = 0;
				accm = 0;
			}
			else
			{
				frame_counter++;
				accm += delta_time;
			}
		}

		void _serealize(serealizer& ser)
		{

		}

		void _deserealize(deserealizer& deser)
		{

		}
	};

	inline void fps_counter_prefab(entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<transform>(vec3{ 32,app::instance().window.get_height() - 64, 0});

		//self.emplace<text_component>(vec2{ 32,64 });

		self.emplace<script_component>(&self).bind<fps_counter_script>();
	}
}