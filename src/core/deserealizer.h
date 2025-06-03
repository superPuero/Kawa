#pragma once
#include <fstream>
#include <string>
#include <typeindex>
#include <iostream>
#include <functional>

#include "components/components.h"
#include "../scene/scene.h"
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

		template<typename T>
		void entity_parse(entity& dest)
		{
			KW_LOG_ERROR("no deserealizer impl for ", typeid(T).name());
		}

		void scene_parese(scene& dest)
		{
			size_t key;
			_input >> key;
			bindable_registrar<scene>::_bindable_factory[key](&dest, nullptr);
		}

		bool is_current(const char* val)
		{
			return _buf == val;
		}

		bool next()
		{
			return static_cast<bool>(_input >> _buf);
		}


		std::ifstream _input;

	private:
		std::string _buf;


	};

	template<>
	inline void deserealizer::entity_parse<UUID>(entity& dest)
	{
		UUID& uuid = dest.emplace<UUID>();
		_input >> uuid.id;
	}

	template<>
	inline void deserealizer::entity_parse<transform>(entity& dest)
	{
		transform& tr = dest.emplace<transform>();

		_input >> tr.position.x  >> tr.position.y  >> tr.position.z ;
		_input >> tr.rotation.x  >> tr.rotation.y  >> tr.rotation.z ;
		_input >> tr.scale.x  >> tr.scale.y >> tr.scale.z;
	}

	template<>
	inline void deserealizer::entity_parse<collider2d>(entity& dest)
	{
		collider2d& col = dest.emplace<collider2d>();
		_input >> col.size.x >> col.size.y >> col.offset.x >> col.offset.y;
	}

	template<>
	inline void deserealizer::entity_parse<sprite2d>(entity& dest)
	{
		sprite2d& sp = dest.emplace<sprite2d>();
		std::string tex_name;

		_input >> tex_name >> sp.size.x >> sp.size.y >> sp.offset.x >> sp.offset.y;	

		sp.tex = &renderer::textures[tex_name];

		char buf[1];
		_input.read(buf, 1);
		_input.read((char*)&sp.texture_coords, sizeof(sp.texture_coords));
	}

	template<>
	inline void deserealizer::entity_parse<script_component>(entity& dest)
	{
		size_t key;
		_input >> key;
		//script_component& sc = dest.emplace<script_component>();
		//bindable_registrar<script_component>::_bindable_factory[key](&sc, &dest);	
		//dest.get<script_component>()._deserealize(*this);
	}

	template<>
	inline void deserealizer::entity_parse<button_component>(entity& dest)
	{
		size_t key;
		_input >> key;

		button_component& bc = dest.emplace<button_component>();

		bindable_registrar<button_component>::_bindable_factory[key](&bc, &dest);
	}

	template<>
	inline void deserealizer::entity_parse<sprite2d_bundle>(entity& dest)
	{
		//_input >> dest.current >> dest.size;

		//dest.bundle = (sprite2d*)std::malloc((sizeof(sprite2d)) * dest.size);

		//for (size_t i = 0; i < dest.size; i++)
		//{
		//	next();
		//	parse(dest.bundle[i]);
		//}
	}
	template<>
	inline void deserealizer::entity_parse<ortho_camera_component>(entity& dest)
	{

		ortho_camera_component& occ = dest.emplace<ortho_camera_component>();

		char buf[1];
		_input.read(buf, 1);

		_input.read((char*)&occ.proj, sizeof(occ.proj));
		_input.read((char*)&occ.view, sizeof(occ.view));

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				KW_LOG("proj",occ.proj[i][j]);
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				KW_LOG("view", occ.view[i][j]);
			}
		}

	}

	template<>
	inline void deserealizer::entity_parse<proj_camera_component>(entity& dest)
	{
		proj_camera_component& cc = dest.emplace<proj_camera_component>();

		cc.proj = glm::perspective
		(
			glm::radians(90.0f),
			(float)app::instance().window.get_width() / (float)app::instance().window.get_height(),
			0.1f,
			1000.0f
		);
	}

	template<>
	inline void deserealizer::entity_parse<physics2d>(entity& dest)
	{
		dest.emplace<physics2d>();
	}

}
