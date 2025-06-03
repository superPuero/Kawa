#pragma once
#include "../core/components/components.h"
#include "../renderer/renderer.h"

namespace kawa
{
	namespace renderer_scripts
	{
		inline void default_transform_marker_render(transform& tr)
		{
			renderer::push_colored_quad(tr, { 10, 10, 0 }, { -5, -5, 0 }, { 0,0,0,0 });
		}

		inline void default_true_collider_render(transform& tr, collider2d& cld)
		{
			renderer::push_colored_quad(tr, { cld.size.x, cld.size.y, 0 }, { cld.offset.x, cld.offset.y, 0 }, { 0,0,0,0 });
		}

		inline void default_text_render(transform& tr, text_component& tc)	
		{
			int32_t pos_x = 0;
			int32_t pos_y = 0;

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
					auto val = (float)ch - 64;
					curr_char_pos.y = 5 - (int)(val / 16);
					curr_char_pos.x = val;
				}

				else if (ch >= 'a' && ch <= 'z')
				{
					auto val = (float)ch - 96;
					curr_char_pos.y = 3 - (int)(val / 16);
					curr_char_pos.x = val;
				}	

				else if (ch == '\n')
				{
					pos_y++;
					pos_x = 0;
					continue;
				}

				else if (ch == ':')
				{
					curr_char_pos = { 10,6 };
				}

				renderer::push_locked_textured_quad
				(
					tr,
					{ tc.size.x, tc.size.y, 0 },
					{ pos_x * tc.size.x + tc.offset.x, - pos_y * tc.size.y + tc.offset.y, 0 },
					renderer::textures["test_font"],
					renderer::textures["test_font"].get_sub_texture_coords(vec2{6, 12}, curr_char_pos)
				);

				pos_x++;

			}
		}

		inline void default_sprite_render(transform& tr, sprite2d& sp)
		{
			renderer::push_textured_quad
			(
				tr,
				{ sp.size.x, sp.size.y, 0 },
				{ sp.offset.x, sp.offset.y, 0 },
				sp.get_texture(),
				sp.texture_coords
			);
		}							 
					
		inline void default_sprite_bundle_render(transform& tr, sprite2d_bundle& spb)
		{
			default_sprite_render(tr, spb.get_current());
		}

		static inline void default_anim_sprite_render(transform& tr, anim_sprite2d& asp) 
		{
			
			//renderer::push_textured_quad
			//(
			//);
		}
	};
}