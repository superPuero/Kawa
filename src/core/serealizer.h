#pragma once
#include <fstream>
#include <string>
#include <typeindex>
#include <iostream>

#include "components.h"

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

	private:
		std::ofstream _output;
		size_t _curr_indent;
	};

	template<>
	inline void serealizer::process(UUID& obj)
	{
		_output << "uuid" << ' ' << obj.id << ' ';
	}

	template<>
	inline void serealizer::process(collider2d& obj)
	{
		_output << "collider2d" << ' ' << obj.size.x << ' ' << obj.size.y << ' ' << obj.offset.x << ' ' << obj.offset.y << ' ';
	}
	template<>
	inline void serealizer::process(transform2d& obj)
	{
		_output << "transform2d" << ' ' << obj.position.x << ' ' << obj.position.y << ' ';
	}

	template<>
	inline void serealizer::process(sprite2d& obj)
	{
		_output << "sprite2d" << ' ' << obj.tex->_name << ' ' << obj.size.x << ' ' << obj.size.y << ' ';
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
		uint64_t script_hash = obj._script_body->get_hash();

		_output << "script_component" << ' ' << script_hash << ' ';
	}

	template<>
	inline void serealizer::process(camera_component& obj)
	{

		_output << "camera_component" << ' ';

		_output.write((char*)&obj.proj, sizeof(obj.proj));
		_output.write((char*)&obj.view, sizeof(obj.view));

		_output << ' ';

	}
}