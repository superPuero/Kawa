#pragma once

#include "../../kawa.h"

namespace kawa
{
	inline void bullet_prefab(entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>();
		self.emplace<physics2d>();
		self.emplace<sprite2d>
			(

			);

	}
}