#pragma once
#include "../../kawa.h"

namespace kawa
{
	//struct asteroid_script : script_component::script_base<asteroid_script>
	KW_SCRIPT_DECL(asteroid_script)
	{
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		bool is_dead = false;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			
		}

		void _serealize(serealizer& ser)
		{
			ser._output << typeid(asteroid_script).hash_code() << ' ';
		}
		void _deserealize(deserealizer& deser)
		{

		}

	};

	void asteroid_prefab(entity& self)
	{
		self.emplace<UUID>();
		auto& tr = self.emplace<transform>
			(
				vec3{rand()%5, rand() % 5, -1}	,
				vec3{ 0, 0, cos(rand())}
			);
		 
		//self.emplace<physics2d>();

		self.emplace<sprite2d>
			(
				&renderer::textures["asteroid"],
				vec2{ 0.5, 0.5 },
				vec2{},
				renderer::textures["asteroid"].get_texture_coords()
			).make_centred();

		self.emplace<script_component>(&self).bind<asteroid_script>();

	}
}