#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../debug/debug.h"

namespace kawa
{
	struct spec
	{
		std::string name = "Default";
		uint32_t width = 1200;
		uint32_t height = 800;
		bool vsync = true;
	};

	class window
	{
	public:
		window(const spec& s);
		~window();

	public:
		inline uint32_t get_width() const noexcept
		{
			return _width;
		}
		inline uint32_t get_height() const noexcept { return _height; };

		void close();
		bool is_key_down(int key);
		bool is_mouse_button_press(int mouse_button);
		bool is_mouse_button_release(int mouse_button);
		void set_input_mode(int mode, int value);
		void get_mouse_pos(float& x, float& y);
		void set_cursor_pos(double x, double y);
		bool window_should_close();
		double time(); 
		void poll_events();
		void swap_buffers();
		static void resize_callback(GLFWwindow* window, int width, int height);


	public:
		uint32_t _height;
		uint32_t _width;
		std::string _name;

		GLFWwindow* _window;
	private:

	};
}

