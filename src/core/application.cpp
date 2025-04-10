#include "application.h"

#include <new>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"

#include "../renderer/Shader.h"
#include "../renderer/renderer.h"

#include "../scene/scene.h"

#include "../debug/debug.h"

namespace kawa
{

	application::application(const char* name, size_t scenes_ammout)
		:	_window(1920, 1080, name),
			_scenes(new scene[scenes_ammout]),
			_current_scene(0)
	{
		KW_LOG("Application initialized");

		application_instance = this;
		window_instange = &_window;
		renderer::init();

	}

	application& application::create(const char* name, size_t scenes_ammout)
	{
		{
			if (!application_instance)
			{
				application_instance = new application(name, scenes_ammout);
				window_instange = &application_instance->_window;
			}
			else
			{
				std::cout << "application instance is already created" << '\n';
			}
			return *application_instance;
		}
	} 

	void application::run()
	{
		main_loop();
	}

	void application::main_loop()
	{
		double start = 0;
		double delta_time = 0;

		renderer::clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });

		while (_window.window_should_close())
		{
			start = _window.time();

			on_update(delta_time);
			on_render();				

			_window.poll_events();

			delta_time = _window.time() - start;
		}

		renderer::shutdown();
	}

	void application::on_update(float delta_time)
	{
		(_scenes +_current_scene)->on_update(delta_time);
	}

	void application::on_render()
	{
		renderer::clear();

		(_scenes + _current_scene)->on_render();

		renderer::draw();

		_window.swap_buffers();
	}

}