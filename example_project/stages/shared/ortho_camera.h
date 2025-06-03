#pragma once
#include "../../kawa.h"

namespace kawa
{
	KW_SCRIPT_DECL(ortho_camera_script)
	{
		//using script_base::script_base;
		KW_SCRIPT_HEADER();

		ortho_camera_component& occ = self.get<ortho_camera_component>();

		transform& tr = self.get<transform>();

		void on_create()
		{

		}

		void on_update(float delta_time)
		{
			glm::mat4 view(1.0f);

			view = glm::translate(view, -tr.position);

			view = glm::mat4(1.0f);

			view = glm::lookAt
			(
				tr.position,
				tr.position + glm::vec3(0, 0, -1),
				glm::vec3(0, 1, 0)
			);

			occ.view = view;

			renderer::shader_map["test"].uniform_mat4("proj", (float*)&occ.proj);
			renderer::shader_map["test"].uniform_mat4("view", (float*)&occ.view);

			if (app::instance().window.is_key_down(GLFW_KEY_W))
			{
				//tr.add_position(vec3{ 0.0,0, -10.0f * delta_time });
				tr.position.z -= 10 * delta_time;

			}
			if (app::instance().window.is_key_down(GLFW_KEY_S))
			{
				//tr.add_position(vec3{ 0,0, 10.0f * delta_time });
				tr.position.z += 10 * delta_time;

			}

		}

		void _serealize(serealizer& ser) {}
		void _deserealize(deserealizer& deser) {}

	};

	inline void ortho_camera_prefab(entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<transform>(vec3{ 0,0,1 });
		self.emplace<ortho_camera_component>(glm::mat4{ 1.0f }, glm::mat4{ 1.0f }).proj = glm::ortho(0.0f, (float)app::instance().window.get_width(), 0.0f, (float)app::instance().window.get_height(), 0.01f, 1000.0f);;



		self.emplace<script_component>(&self).bind<ortho_camera_script>();
	}
}