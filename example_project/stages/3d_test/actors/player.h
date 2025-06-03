#pragma once
#include "../../kawa.h"


namespace test3d
{
	using namespace kawa;

	KW_SCRIPT_DECL(player_script)
	{
		KW_SCRIPT_HEADER();

		transform& tr = self.get<transform>();

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			glm::vec3 forward = glm::vec3
			(
				-sin(tr.rotation.y),
				0.0f,
				-cos(tr.rotation.y)
			);

			if (app::instance().window.is_key_down(GLFW_KEY_W))
			{
				tr.position += forward * 5.0f * delta_time;
			}
			if (app::instance().window.is_key_down(GLFW_KEY_S))
			{
				tr.position -= forward * 5.0f * delta_time;
			}

			if (app::instance().window.is_key_down(GLFW_KEY_D))
			{
				tr.rotation.y -= 5.0f * delta_time; 
			}
			if (app::instance().window.is_key_down(GLFW_KEY_A))
			{
				tr.rotation.y += 5.0f * delta_time;
			}
		}

		void _serealize(serealizer & ser)
		{
		
		}

		void _deserealize(deserealizer & deser)
		{

		}

	};

	inline void player_prefab(entity& self)
	{
		self.emplace<UUID>();

		auto& cc = self.emplace<proj_camera_component>();

		cc.proj = glm::perspective
		(
			glm::radians(cc.fov),
			(float)app::instance().window.get_width() / (float)app::instance().window.get_height(),
			0.1f,
			1000.0f
		);

		cc.primary = true;

		self.emplace<transform>(vec3{0,0,-1}, vec3{0, glm::radians(180.0f), 0});

		self.emplace<script_component>(&self).bind<player_script>();
	}
}