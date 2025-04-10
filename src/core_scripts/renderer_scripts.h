#pragma once
#include "../core/components.h"
#include "../renderer/renderer.h"

namespace kawa
{
	struct renderer2d_scripts
	{
		static inline void default_transform_marker_render(transform& tr)
		{
			renderer::push_colored_quad(tr, { 10, 10, -1 }, { -5, -5, -1 }, { 0,0,0,0 });
		}

		static inline void default_true_collider_render(UUID& uuid, transform& tr, collider2d& cld)
		{
			renderer::push_colored_quad(tr, { cld.size.x, cld.size.y, -1 }, { cld.offset.x, cld.offset.y, -1 }, { 0,0,0,0 });
		}

		static inline void default_text_render(transform& tr, text_component& tc)	
		{
			uint32_t pos = 0;
			glm::vec2 curr_char_pos = { 0,0 };

			for (char ch : tc.data)
			{
				if (ch == ' ')
				{
					curr_char_pos.y = 7;
					curr_char_pos.x = 0;
				}

				else if (ch >= '0' && ch <= '9')
				{
					curr_char_pos.y = 6;
					curr_char_pos.x = (float)ch - 48;
				}

				else if (ch >= 'A' && ch <= 'Z')
				{
					constexpr auto v = 'P' - 64;
					auto val = (float)ch - 64;
					curr_char_pos.y = 5 - (val / 16);
					curr_char_pos.x = val;
				}

				else if (ch >= 'a' && ch <= 'z')
				{
					auto val = (float)ch - 96;
					curr_char_pos.y = 3 - (val / 16);
					curr_char_pos.x = val;
				}
			

				renderer::push_textured_quad
				(
					tr,
					{ tr.position.x + pos * tc.size.x, tr.position.y, -1 },
					{ tc.size.x, tc.size.y, -2 },
					renderer::textures["test_font"],
					renderer::textures["test_font"].get_sub_texture_coords(vec2{6, 12}, curr_char_pos)
				);

				pos++;
			}
		}

		static inline void default_sprite_render(sprite2d& sp, transform& tr)
		{
			renderer::push_textured_quad
			(
				tr,
				{ sp.size.x, sp.size.y, -1 },
				{ sp.offset.x, sp.offset.y, -1},
				sp.get_texture(),
				sp.texture_coords
			);
		}
					
		static inline void default_sprite_bundle_render(sprite2d_bundle& spb, transform& tr)
		{
			default_sprite_render(spb.get_current(), tr);
		}


		static inline void default_tile_map_render(transform& tr, tile_map2d& sp)	
		{
			for (size_t i = 0; i < sp.map_height; i++)
			{
				for (size_t j = 0; j < sp.map_width; j++)
				{
					renderer::push_textured_quad
					(			
						tr,
						{ sp.tile_screen_size.x * j, sp.tile_screen_size.y * i, -1 },
						{ sp.tile_screen_size.x, sp.tile_screen_size.y, 0 }	,
						sp.tex_source,
						sp.tile_texture_coords[j + (i * sp.map_width)]
					);
				}
			}
		}

		//static inline void default_anim_sprite_render(transform& tr, anim_sprite2d& sp) 
		//{
		//	renderer::push_true_quad
		//	(
		//		sp.tex_source,
		//		sp.frames_texture_coords[sp.frame_current],
		//		{ sp., 0, -1 },
		//		{ sp.frame_screen_size.x, sp.frame_screen_size.y, 0 },
		//		tr
		//	);
		//}

		//static inline void default_anim_sprite_bundle_render(transform& tr, anim_sprite2d_bundle& sp)
		//{
		//	renderer::push_true_quad
		//	(
		//		sp.current().tex_source,
		//		sp.current().frames_texture_coords[sp.current().frame_current],
		//		{ 0, 0, -1 },
		//		{ sp.current().frame_screen_size.x, sp.current().frame_screen_size.y, 0 },
		//		tr
		//	);
		//}
	};
}