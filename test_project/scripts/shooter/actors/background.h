#pragma once

#include "../../kawa.h"

namespace kawa
{
	inline void background_prefab(scene& context, entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>(vec3{ application::application_instance->_window.win_width/2, application::application_instance->_window.win_height/2, -1 });

		self.emplace<sprite2d>
			(
				&renderer::textures["stars_background"],
				vec2{ renderer::textures["stars_background"]._width, renderer::textures["stars_background"]._height },
				vec2{},
				renderer::textures["stars_background"].get_texture_coords()
			).make_centred();


		self.get<transform>().scale.x = 5;
		self.get<transform>().scale.y = 5;

	}
}