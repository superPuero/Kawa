#pragma 
#include "../../kawa.h"

#include "actors.h"


namespace  game 
{
	using namespace kawa;
		
	struct script : scene_logic_body
	{
		bool collider_view = true;

		void on_create()
		{
			self.query([](script_component& sc) { sc.on_create(); });
		}

		void on_update(float delta_time)
		{
			if (application::application_instance->_window.is_key_down(GLFW_KEY_V))
			{
				collider_view = true;
			}
			if (application::application_instance->_window.is_key_down(GLFW_KEY_C))
			{
				collider_view = false;
			}

			if (application::application_instance->_window.is_key_down(GLFW_KEY_3))
			{
				self.serealize("menu.txt");
			}

			self.query([](float delta_time, script_component& sc) { sc.on_update(delta_time); }, delta_time);
			self.cleanup();
		}

		void on_render()
		{
			if (collider_view)
			{
				self.query(renderer2d_scripts::default_collider_render);
			}
			self.query(renderer2d_scripts::default_sprite_render);
			self.query(renderer2d_scripts::default_tile_map_render);
			self.query(renderer2d_scripts::default_anim_sprite_render);
			self.query(renderer2d_scripts::default_anim_sprite_bundle_render);
			self.query(renderer2d_scripts::default_text_render);


			if (application::application_instance->_window.is_key_down(GLFW_KEY_H))
			{
				self.serealize("test.txt");
			}
		}
	};

	static void _assets()
	{
		renderer::load_texture("assets/Texture/TX Player.png", "test");
		renderer::load_texture("assets/Texture/TX Plant.png", "test2");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Idle.png", "knight_idle_right");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Idle.png", "knight_idle_left", true);
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Run.png", "knight_run_right");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Run.png", "knight_run_left", true);
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_TurnAround.png", "knight_turn_left");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_TurnAround.png", "knight_turn_right", true);
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Jump.png", "knight_jump_right");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Jump.png", "knight_jump_left", true);
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Fall.png", "knight_fall_right");
		renderer::load_texture("assets/FreeKnight_v1 (1)/Colour1/Outline/120x80_PNGSheets/_Fall.png", "knight_fall_left", true);
		renderer::load_texture("assets/Legacy-Fantasy - High Forest 2.3/Assets/Tiles.png", "tiles");
	}

	static void prefab(scene& self)
	{
		_assets();
		
		self.entity_from_prefab(player1_prefab::prefab);
		self.entity_from_prefab(player2_prefab::prefab);
		self.entity_from_prefab(text_prefab::prefab);
		self.entity_from_prefab(counter_prefab::prefab);
		self.entity_from_prefab(block_prefab::prefab);
		self.entity_from_prefab(block2_prefab::prefab);
		self.entity_from_prefab(counter_prefab::prefab);
		self.entity_from_prefab(ortho_camera_prefab::prefab);

		for (size_t i = 0; i < 100000; i++)
		{
			self.entity_from_prefab(coin_prefab::prefab);
		}

		self.bind<script>();

		self.on_create();

	}
}