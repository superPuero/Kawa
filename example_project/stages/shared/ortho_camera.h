#pragma once
#include "../../kawa.h"

namespace kawa
{
	struct ortho_camera_script : script_component::script_base
	{
		using script_base::script_base;

		camera_component& cc = self.get<camera_component>();
		transform& tr = self.get<transform>();

		void on_create()
		{
			renderer::shader_map["test"].uniform_mat4("proj", (float*)&cc.proj);
			renderer::shader_map["test"].uniform_mat4("view", (float*)&cc.view);
		}

		void on_update(float delta_time)
		{
			glm::mat4 view(1.0f);
			view = glm::translate(view, -tr.position);
			cc.view = view;
			renderer::shader_map["test"].uniform_mat4("view", (float*)&cc.view);
		}
	};

	inline void ortho_camera_prefab(entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<camera_component>(glm::mat4{ 1.0f }, glm::mat4{ 1.0f });
		self.emplace<transform>(vec3{ 0,0,-1 });

		self.get<camera_component>().proj = glm::ortho(0.0f, (float)application::application_instance->_window.win_width, 0.0f, (float)application::application_instance->_window.win_height, 0.01f, 1000.0f);

		self.emplace<script_component>().bind<ortho_camera_script>(self);
	}
}