#pragma once
#include <fstream>
#include <string>
#include <typeindex>
#include <iostream>

#include "components/components.h"
#include "../scene/scene.h"

namespace kawa
{
	class serealizer
	{

	public:
		serealizer(const char* filepath)
			: _output(filepath),
			_curr_indent(0)
		{}

		template<typename T>
		void process(T& obj)
		{
			std::cout << "serialization is not implemented for " << typeid(T).name();
		}

		template<typename T>
		void process_if_exsist(T* obj)
		{
			if (obj)
			{
				process(*obj);
			}
		}

		template<typename T>
		void append(const T& val)
		{
			_output << val << ' ';
		}

		template<typename T>
		void write(const T& val)
		{
			_output.write((char*)&val, sizeof(T));
		}

		std::ofstream _output;
	private:
		size_t _curr_indent;

	};
	
	template<>
	inline void serealizer::process(scene& obj)
	{
		_output << "scene" << ' ';
		_output << obj.get_poly_hash() << ' ';
	}

	template<>
	inline void serealizer::process(UUID& obj)
	{
		_output << "uuid" << ' ' << obj.id << ' ';
	}

	template<>
	inline void serealizer::process(transform& obj)
	{
		_output << "transform" << ' ' ;
		_output << obj.position.x << ' ' << obj.position.y << ' ' << obj.position.z << ' ';
		_output << obj.rotation.x << ' ' << obj.rotation.y << ' ' << obj.rotation.z << ' ';
		_output << obj.scale.x << ' ' << obj.scale.y << ' ' << obj.scale.z << ' ';
	}

	template<>
	inline void serealizer::process(collider2d& obj)
	{
		_output << "collider2d" << ' ' << obj.size.x << ' ' << obj.size.y << ' ' << obj.offset.x << ' ' << obj.offset.y << ' ';
	}

	template<>
	inline void serealizer::process(sprite2d& obj)
	{
		_output << "sprite2d" << ' ' << obj.tex->_name << ' ' << obj.size.x << ' ' << obj.size.y << ' ';
		_output << obj.offset.x << ' ' << obj.offset.y << ' ';

		_output.write((char*)&obj.texture_coords, sizeof(obj.texture_coords));

		_output << ' ';
	}

	template<>
	inline void serealizer::process(sprite2d_bundle& obj)
	{
		_output << "sprite2d_bundle" << ' ' << obj.current << ' ' << obj.size << ' ';

		for (size_t i = 0; i < obj.size; i++)
		{
			process(obj.bundle[i]);
		}
	}

	template<>
	inline void serealizer::process(script_component& obj)
	{
		_output << "script_component" << ' ';
		_output << obj.get_poly_hash() << ' ';
		obj._serealize(*this);
	}

	template<>
	inline void serealizer::process(button_component& obj)
	{
		_output << "button_component" << ' ';
		_output << obj._button_body->get_hash() << ' ';
	}

	template<>
	inline void serealizer::process(ortho_camera_component& obj)
	{

		_output << "ortho_camera_component" << ' ';

		_output.write((char*)&obj.proj, sizeof(obj.proj));
		_output.write((char*)&obj.view, sizeof(obj.view));

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				KW_LOG("proj", obj.proj[i][j]);
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				KW_LOG("view", obj.view[i][j]);
			}
		}

		_output << ' ';
	}



	template<>
	inline void serealizer::process(physics2d& obj)
	{
		_output << "physics2d" << ' ';
	}

	template<>
	inline void serealizer::process(proj_camera_component& obj)
	{

		_output << "proj_camera_component" << ' ';

		//_output.write((char*)&obj.proj, sizeof(obj.proj));

		//_output << ' ';
	}

}