#pragma once
#include "../../kawa.h"


namespace test3d
{
	using namespace kawa;

	KW_SCRIPT_DECL(square_script)
	{
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();

		float s;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			//s += delta_time;				

			//tr.rotation.x = sin(s);
			//tr.rotation.y = cos(s);

			//tr.scale.y = cos(s);

			//KW_LOG("SQUARE UP");
		}

		void _serealize(serealizer & ser)
		{
		}

		void _deserealize(deserealizer & deser)
		{

		}

	};

	inline void square_prefab(entity& self)
	{
		self.emplace<UUID>();

		self.emplace<transform>();

		self.emplace<sprite2d_bundle>()
			.add_centered(&renderer::textures["white"], vec2{ 0.5f, 0.5f}, renderer::textures["white"].get_texture_coords());

		self.emplace<script_component>(&self).bind<square_script>();
	}
}