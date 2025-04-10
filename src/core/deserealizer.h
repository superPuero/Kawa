#pragma once
#include <fstream>
#include <string>
#include <typeindex>
#include <iostream>

#include "components.h"
#include "../renderer/renderer.h"
#include "../debug/debug.h"

namespace kawa
{

	class deserealizer
	{
	public:
		deserealizer(const char* filepath)
			: _input(filepath)
		{}

		template<typename T, typename...Args>
		void parse(T& dest, Args&&...args)
		{
			KW_LOG_ERROR("no deserealizer impl for ", typeid(T).name());
		}

		bool is_current(const char* val)
		{
			return _buf == val;
		}
		bool next()
		{
			return static_cast<bool>(_input >> _buf);
		}

	private:
		std::ifstream _input;
		std::string _buf;
	};

	template<>
	inline void deserealizer::parse(UUID& dest)
	{
		_input >> dest.id;
	}

	template<>
	inline void deserealizer::parse(collider2d& dest)
	{
		_input >> dest.size.x >> dest.size.y >> dest.offset.x >> dest.offset.y;
	}

	template<>
	inline void deserealizer::parse(sprite2d& dest)
	{
		std::string tex_name;
		_input >> tex_name >> dest.size.x >> dest.size.y;

		dest.tex = &renderer::textures[tex_name];

		dest.texture_coords = dest.tex->get_texture_coords();
	}

	template<>
	inline void deserealizer::parse(sprite2d_bundle& dest)
	{
		_input >> dest.current >> dest.size;

		dest.bundle = (sprite2d*)std::malloc((sizeof(sprite2d)) * dest.size);

		for (size_t i = 0; i < dest.size; i++)
		{
			next();
			parse(dest.bundle[i]);
		}
	}

	//template<>
	//inline void deserealizer::parse(script_component& dest, scene& s, entity& e)
	//{
	//	uint64_t script_hash;
	//	_input >> script_hash;

	//	std::cout << "here" << '\n';

	//	if (script_hash == typeid(main_menu::play_button_script).hash_code())
	//	{
	//		dest._script_body = new main_menu::play_button_script(s, e);
	//	}

	//	else if (script_hash == typeid(main_menu::home_button_script).hash_code())
	//	{
	//		dest._script_body = new main_menu::home_button_script(s, e);
	//	}

	//	else if (script_hash == typeid(main_menu::bit_script).hash_code())
	//	{
	//		dest._script_body = new main_menu::bit_script(s, e);
	//	}

	//	else if (script_hash == typeid(main_menu::ortho_camera_script).hash_code())
	//	{
	//		dest._script_body = new main_menu::ortho_camera_script(s, e);
	//	}
	//}

	template<>
	inline void deserealizer::parse(camera_component& dest)
	{
		_input.read((char*)&dest.proj, sizeof(dest.proj));

		_input.read((char*)&dest.view, sizeof(dest.view));

	}
}
