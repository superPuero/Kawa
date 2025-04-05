#pragma once
#include "../../kawa.h"


namespace main_menu
{
	using namespace kawa;
	struct bit_script : script_component::script_base
	{
		using script_base::script_base;

		transform2d& transfrom = self.get<transform2d>();

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
			application::application_instance->_window.get_mouse_pos(transfrom.position.x, transfrom.position.y);
		}
	};


	struct bit_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id;

			self.emplace<tag_component>(NONE);

			self.emplace<transform2d>(vec2{200,200});
			self.emplace<collider2d>(vec2{ 200,  200 });

			self.emplace<script_component>().bind<bit_script>(scene, self);
		}
	};

	struct play_button_script : script_component::script_base
	{
		using script_base::script_base;

		transform2d& transform = self.get<transform2d>();
		collider2d& cld = self.get<collider2d>();
		sprite2d_bundle& spb = self.get<sprite2d_bundle>();

		bool is_hovered()
		{
			vec2 mouse_pos;

			application::application_instance->_window.get_mouse_pos(mouse_pos.x, mouse_pos.y);

			return collider2d::is_point_inside(mouse_pos, transform, cld);
		}

		void on_create()
		{
		};

		void on_update(float delta_time)
		{			
			if (is_hovered())
			{
				spb.current = 1;
				if (application::application_instance->_window.is_mouse_button_press(GLFW_MOUSE_BUTTON_1))
				{
					spb.current = 2;
					application::application_instance->set_scene(1);							
				}
			}
			else
			{
				spb.current = 0;
			}
		}
	};


	struct play_button_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>();
			self.emplace<tag_component>(BUTTON);
			self.emplace<transform2d>(vec2{ application::application_instance->_window.win_width/2 - 716/2 , 600.0f });
			self.emplace<collider2d>(vec2{ 716,  160 });

			self.emplace<sprite2d_bundle>(3)
									.put(0, &renderer::textures["play_normal"], renderer::textures["play_normal"].get_texture_coords(), vec2{ 716.0f, 160.0f })
									.put(1, &renderer::textures["play_highlighted"], renderer::textures["play_highlighted"].get_texture_coords(), vec2{ 716.0f, 160.0f })
									.put(2, &renderer::textures["play_pressed"], renderer::textures["play_pressed"].get_texture_coords(), vec2{ 716.0f, 160.0f });
			
			self.emplace<script_component>().bind<play_button_script>(scene, self);
		}
	};


	struct home_button_script : script_component::script_base
	{	
		using script_base::script_base;

		transform2d& tr  = self.get<transform2d>();
		collider2d& cld = self.get<collider2d>();
		sprite2d_bundle& spb = self.get<sprite2d_bundle>();

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
			float mouse_x;
			float mouse_y;

			application::application_instance->_window.get_mouse_pos(mouse_x, mouse_y);

			if (collider2d::is_point_inside(vec2{ mouse_x, mouse_y }, tr, cld))
			{
				spb.current = 1;
				if (application::application_instance->_window.is_mouse_button_press(GLFW_MOUSE_BUTTON_1))
				{
					spb.current = 2;
					application::application_instance->exit();
				}
			}
			else
			{
				spb.current = 0;
			}		

		}
	};

	struct home_button_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();

			self.emplace<tag_component>(BUTTON);

			self.emplace<transform2d>(vec2{ application::application_instance->_window.win_width / 2 - 716 / 2 , 300.0f });
			self.emplace<collider2d>(vec2{ 716,  160 });

			self.emplace<sprite2d_bundle>(3)
				.put(0, &renderer::textures["home_normal"], renderer::textures["home_normal"].get_texture_coords(), vec2{ 716.0f, 160.0f })
				.put(1, &renderer::textures["home_highlighted"], renderer::textures["home_highlighted"].get_texture_coords(), vec2{ 716.0f, 160.0f })
				.put(2, &renderer::textures["home_pressed"], renderer::textures["home_pressed"].get_texture_coords(), vec2{ 716.0f, 160.0f });		

			self.emplace<script_component>().bind<home_button_script>(scene, self);
		}
	};

	struct ortho_camera_script : script_component::script_base
	{

		using script_base::script_base;

		camera_component& cc = self.get<camera_component>();

		void on_create()
		{
			renderer::shader_map["test"].uniform_mat4("view", (float*)&cc.view);
			renderer::shader_map["test"].uniform_mat4("proj", (float*)&cc.proj);
		}

		void on_update(float delta_time)
		{

		}
	};

	struct ortho_camera_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<camera_component>(glm::mat4{ 1.0f }, glm::mat4{ 1.0f });
			self.emplace<transform3d>(vec3{ 0,0,0 }, vec3{ glm::radians(-90.0f),0,0 });
			self.emplace<physics3d>(vec3{ 0,0,0 });

			self.get<camera_component>().proj = glm::ortho(0.0f, (float)application::application_instance->_window.win_width, 0.0f, (float)application::application_instance->_window.win_height, 0.01f, 1000.0f);

			self.emplace<script_component>().bind<ortho_camera_script>(scene, self);
		}
	};
};