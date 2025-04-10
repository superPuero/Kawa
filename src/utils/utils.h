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
    T& as(void* ptr)
    {
        return *static_cast<T*>(ptr);
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

    struct vertex
    {
        glm::vec3 position;
        glm::vec2 texture_coords;
        int texture_id;       
    };
}