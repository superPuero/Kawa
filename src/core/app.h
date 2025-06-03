#pragma once

#include <string>
#include "../scene/scene.h"

#include "macros.h"
#include "window.h"
#include "data_structures/dyn_array.h"

namespace kawa
{
	class app
	{

	public:

	public:
		static app& create(const spec& s);
		static void shutdown();

		static app& instance();

	public:
		void run();
		void set_scene(const std::string& name);
		void exit();

	public:
		app& with_scene(std::string&& name, void(prefab)(scene&));


	public:
		window window;

	private:
		app(const spec& s);

		app(app&&) noexcept = delete;
		app& operator=(app&&) noexcept = delete;

		app(const app&) = delete;
		app& operator=(const app&) = delete;

		~app();

	private:
		void main_loop();
		void on_update(float delta_time);
		void on_render();

	private:
		static inline app* _instance;

	private:
		dyn_array<kawa::scene> _scenes;
		dyn_array<std::string> _scene_identifiers;
		size_t _scenes_occupied;

		size_t _current_scene;

		bool _running;
	};

}
