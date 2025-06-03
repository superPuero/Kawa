#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glm.hpp>
#include "../../vendors/stb/stb_image.h"

#include "../debug/debug.h"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;

namespace kawa
{
    template<typename T>
    inline T lerp(const T& start, const T& end, float delta, float (easin_fn)(float))
    {
        return start + (end - start) * easin_fn(delta);
    }

    inline float ease_in_quad(float t) 
    {
        return t * t;
    }

    inline float ease_in_out_sine(float t) 
    {
        return -0.5f * (std::cos(3.1415 * t) - 1.0f);
    }

    template<typename T>
    inline constexpr T& as(void* ptr) noexcept
    {
        return *static_cast<T*>(ptr);
    }

    template<typename T>
    inline const char* type_name() noexcept
    {
        return typeid(T).name();
    }

    template<typename T>
    inline size_t static_type_id() noexcept
    {
        static size_t counter = 0;

        static size_t id = counter++;

        return id;
    }

    constexpr glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	static std::string read_to_str(const char* filepath)
	{
        std::ifstream file(filepath, std::ios::binary);

        if (!file) 
        {
            KW_LOG_ERROR("Unbale to open file", filepath);
        }

        file.seekg(0, std::ios::end);        

        size_t size = file.tellg();  
        std::string buffer(size, '\0');    

        file.seekg(0);                        
        file.read(buffer.data(), size);

        return buffer;
	}
}