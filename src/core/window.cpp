#include <iostream>                
#include "window.h"
#include "../debug/debug.h"

namespace kawa
{
    window::window(const spec& s)
    {
        KW_LOG("Initializing window");
        KW_LOG("Name:", s.name);
        KW_LOG("Width:", s.width);
        KW_LOG("Height:", s.height);

        std::boolalpha(std::cout);
        KW_LOG("Vsync:", s.vsync);
        std::noboolalpha(std::cout);

        _width = s.width;
        _height = s.height;
        _name = s.name;

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if(s.vsync)
        { 
            glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        }

        _window = glfwCreateWindow(_width, _height, _name.c_str(), NULL, NULL);

        glfwMakeContextCurrent(_window);

        if(s.vsync)
        { 
            glfwSwapInterval(1);
        } 

        gladLoadGL();

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
        double tx, ty;
        glfwGetCursorPos(_window, &tx, &ty);

        x = tx;
        y = ty;
    }
    void window::set_cursor_pos(double x, double y)
    {
        glfwSetCursorPos(_window, x, y);
    }

    bool window::window_should_close()
    {
        return glfwWindowShouldClose(_window);
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
        //win_height = width;
        //win_height = height;
    }
}
