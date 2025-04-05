#pragma once

namespace kawa
{
	class scene;

	struct scene_logic_body
	{
		scene& self;

		void on_create() {};
		void on_update(float delta_time) {};
		void on_render() {};
	};
}