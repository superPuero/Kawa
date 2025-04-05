#include <iostream>                
#include "window.h"

namespace kawa
{
    window::window(uint32_t width, uint32_t height, const char* name)
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

        _window = glfwCreateWindow(width, height, name, NULL, NULL);

        glfwMakeContextCurrent(_window);

        glfwSwapInterval(1);

        gladLoadGL();

        win_width = width;
        win_height = height;

        glfwSetWindowSizeCallback(_window, &resize_callback);
    }

    window::~window()
    {
        glfwDestroyWindow(_window);
        _window = nullptr;

    }

    void window::close()
    {
        glfwSetWindowShouldClose(_window, true);
    }

    bool window::is_key_down(int key)
    {
        return glfwGetKey(_window, key);
    }

    bool window::is_mouse_button_press(int mouse_button)
    {
        return glfwGetMouseButton(_window, mouse_button) == GLFW_PRESS;
    }

    bool window::is_mouse_button_release(int mouse_button)
    {
        return glfwGetMouseButton(_window, mouse_button) == GLFW_RELEASE;
    }

    void window::set_input_mode(int mode, int value)
    {
        glfwSetInputMode(_window, mode, value);
    }

    void window::get_mouse_pos(float& x, float& y)
    {
        double t_x;
        double t_y;

        glfwGetCursorPos(_window, &t_x, &t_y);

        x = t_x;
        y = (t_y - win_height) * -1;

    }
    void window::set_cursor_pos(double x, double y)
    {
        glfwSetCursorPos(_window, x + win_width / 2, y + win_height / 2);
    }

    bool window::window_should_close()
    {
        return !glfwWindowShouldClose(_window);
    }

    double window::time()
    {
        return glfwGetTime();
    }

    void window::poll_events()
    {
        glfwPollEvents();
    }

    void window::swap_buffers()
    {
        glfwSwapBuffers(_window);
    }

    void window::resize_callback(GLFWwindow* window, int width, int height)
    {
        //win_width = width;
        //win_height = height;
    }
}
