#pragma once
#include "macros.h"
#include "window.h"
#include "../scene/scene.h"

namespace kawa
{
	class scene;
			
	class application
	{
	public:
		static application& create(const char* name, size_t scenes_ammout);

	public:
		inline static application* application_instance;
		inline static window* window_instange;

	public:
		void run();

		void set_scene(size_t scene_id)
		{
			_current_scene = scene_id;
		}
		void exit()
		{
			_window.close();
		}

		void scene_from_prefab(size_t scene_id, void(prefab)(scene&))
		{
			_scenes[scene_id].prefab(prefab);
		}


	private:
		application(const char* name, size_t scenes_ammout);

	public:
		window _window;

	private:
		void main_loop();
		void on_update(float delta_time);
		void on_render();

	private:
		scene* _scenes;
		size_t _current_scene;

	};

}
