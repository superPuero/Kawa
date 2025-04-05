#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace kawa
{
	class window
	{
	public:
		window(uint32_t width, uint32_t height, const char* name);				
		~window();

	public:
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
		uint32_t win_height;
		uint32_t win_width;

		GLFWwindow* _window;
	private:

	};
}

