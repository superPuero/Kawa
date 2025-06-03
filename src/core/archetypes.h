#pragma once

#include "components/components.h"

namespace kawa
{

	inline void default_prototype(entity& self)
	{
		self.emplace<UUID>();
		self.emplace<transform>();
	}

	inline void button_prototype(entity& self)
	{
		self.apply_prefab(default_prototype);
		self.emplace<collider2d>();
		self.emplace<button_component>();
	}
}