#pragma once
#include "../../kawa.h"

namespace kawa
{
	KW_SCRIPT_DECL(perspective_camera_script)
	{
		KW_SCRIPT_HEADER();

		proj_camera_component& cc = self.get<proj_camera_component>();
		transform& tr = self.get<transform>();

		bool locked = false;

		void on_create()
		{
			tr.rotation.y = glm::radians(180.0f);
		}

		void on_update(float delta_time)
		{
			if (app::instance().window.is_mouse_button_press(GLFW_MOUSE_BUTTON_1) && !locked)
			{
				locked = true;
				app::instance().window.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}

			if (app::instance().window.is_key_down(GLFW_KEY_ESCAPE) && locked)
			{						
				locked = false;
				app::instance().window.set_input_mode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);				
			}

			if(locked)
			{

				float mouse_x, mouse_y;
				
				app::instance().window.get_mouse_pos(mouse_x, mouse_y);
				
				app::instance().window.set_cursor_pos
				(
					app::instance().window.get_height() / 2.0f,
					app::instance().window.get_width() / 2.0f
				);				
			}

			cc.view = glm::inverse(tr.get_mat4());

			renderer::shader_map["test"].uniform_mat4("proj", (float*)&cc.proj);
			renderer::shader_map["test"].uniform_mat4("view", (float*)&cc.view);

			if (app::instance().window.is_key_down(GLFW_KEY_W))
			{
				tr.position.z -= 5.0f * delta_time;
			}
			if (app::instance().window.is_key_down(GLFW_KEY_S))
			{
				tr.position.z += 5.0f * delta_time;
			}
		}

		void _serealize(serealizer& ser) 
		{
			ser._output << typeid(perspective_camera_script).hash_code() << ' ';
		}

		void _deserealize(deserealizer& deser) 
		{
			
		}

	};

	inline void perspective_camera_prefab(entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<proj_camera_component>();
		self.emplace<transform>(vec3{ 0, 0, -1 });

		proj_camera_component& cc = self.get<proj_camera_component>();

		cc.proj = glm::perspective				
		(
			glm::radians(cc.fov),
			(float)app::instance().window.get_width() /(float)app::instance().window.get_height(),
			0.1f,
			1000.0f
		);

		self.emplace<script_component>(&self).bind<perspective_camera_script>();		
	}
}