#include "app.h"

#include <new>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"

#include "../renderer/renderer.h"

#include "../scene/scene.h"

#include "../debug/debug.h"

namespace kawa
{
	app::app(const spec& s)
		:	window(s)
		,	_scenes_occupied(0)
		,	_current_scene(0)
	{
		renderer::init();
	}

	app& app::create(const spec& s)
	{
		KW_ASSERT(!_instance);

		_instance = new app(s);

		return *_instance;
	}

	void app::shutdown()
	{
		KW_ASSERT(_instance);

		delete _instance;
	}

	app& app::instance()
	{
		KW_ASSERT(app::_instance);

		return *app::_instance;
	}

	void app::run()
	{
		_running = true;

		main_loop();

		app::shutdown();
	}

	void app::set_scene(const std::string& name)
	{
		size_t i = 0;

		for (const auto& n : _scene_identifiers)
		{
			if (n == name)
			{
				_current_scene = i;
				return;
			}
			i++;
		}

		KW_ASSERT_MSG(false, "no scene named", name, "exsists");

	}

	void app::exit()
	{
		_running = false;
	}

	app& app::with_scene(std::string&& name, void(prefab)(scene&))
	{
		_scene_identifiers.emplace_back(name);

		scene& s = _scenes.emplace_back();

		s.prefab(prefab);
		s.on_create();

		_scenes_occupied++;

		return *this;
	}

	app::~app()
	{
		renderer::shutdown();
	}

	void app::main_loop()
	{
		double start = 0;
		double delta_time = 0;

		while (_running)
		{
			_running = !window.window_should_close();

			start = window.time();

			on_update(delta_time);			
			on_render();

			window.poll_events();

			delta_time = window.time() - start;
		}

		renderer::shutdown();
	}

	void app::on_update(float delta_time)
	{
		_scenes[_current_scene].on_update(delta_time);
	}

	void app::on_render()
	{
		renderer::clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });

		renderer::clear();

		_scenes[_current_scene].on_render();

		renderer::draw();

		window.swap_buffers();
		
	}

}