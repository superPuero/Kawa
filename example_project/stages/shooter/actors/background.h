#pragma once

#include "../../kawa.h"

namespace kawa
{
	inline void background_prefab(entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>(vec3{0,0,0 });

		self.emplace<sprite2d>
			(
				&renderer::textures["stars_background"],
				vec2{ renderer::textures["stars_background"]._width/100, renderer::textures["stars_background"]._height/100 },
				vec2{},
				renderer::textures["stars_background"].get_texture_coords()
			).make_centred();


		self.get<transform>()._scale.x = 5;
		self.get<transform>()._scale.y = 5;

	}
}