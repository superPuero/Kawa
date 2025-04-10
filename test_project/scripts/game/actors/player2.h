#pragma once
#include "../../kawa.h"
#include "counter.h"

namespace kawa
{
	inline vec2 anim_test2[10] =
	{
		vec2{0,0}, vec2{1,0}, vec2{2,0}, vec2{3,0},
		vec2{4,0}, vec2{5,0}, vec2{6,0}, vec2{7,0},
		vec2{8,0}, vec2{9,0}
	};

	inline vec2 anim_test_22[3] =
	{
		vec2{0,0}, vec2{1,0}, vec2{2,0}
	};


	struct player2_script : script_component::script_base
	{
		using script_base::script_base;

		UUID& uuid = self.get<UUID>();
		transform2d& tr = self.get<transform2d>();
		physics2d& ph = self.get<physics2d>();
		collider2d& cld = self.get<collider2d>();
		anim_sprite2d_bundle& asb = self.get<anim_sprite2d_bundle>();


		void on_create()
		{

		}


		void on_update(float delta_time)
		{
			ph.force = { 0, 0 };

			if (application::application_instance->_window.is_key_down(GLFW_KEY_A))
			{
				ph.force.x = -50;
			}

			if (application::application_instance->_window.is_key_down(GLFW_KEY_D))
			{
				ph.force.x = 50;
			}

			if (application::application_instance->_window.is_key_down(GLFW_KEY_W) && ph.on_ground)
			{
				ph.force.y = 1500;
				ph.on_ground = false;
			}

			ph.velocity += ph.force / ph.mass;
			ph.velocity.x *= 0.95f;

			if (ph.velocity.x < 15 && ph.velocity.x > 0)
			{
				ph.velocity.x = 0;
			}
			else if (ph.velocity.x > -15 && ph.velocity.x < 0)
			{
				ph.velocity.x = 0;
			}

			if (ph.velocity.x > 2000)
			{
				ph.velocity.x = 2000;
			}
			else if (ph.velocity.x < -2000)
			{
				ph.velocity.x = -2000;
			}

			calculate(delta_time);
		}

		inline void calculate(float delta_time)
		{
			ph.velocity.x -= ph.mass * delta_time;

			tr.position.x += ph.velocity.x * delta_time;

			context.query_with_each
			(
				[&](entity& other, UUID& _uuid, transform2d& _tr, collider2d& _cld)
				{
					if (uuid != _uuid)
					{
						if (collider2d::is_colliding(tr, _tr, cld, _cld))
						{
							auto _tag = other.get_if_has<tag_component>();

							if (_tag)
							{
								if (_tag->value == COIN)
								{
									context.query
									(
										[](script_component& scr, tag_component& tag)
										{
											if (tag.value == COIN_COUNTER) scr.get_body<counter_script>().counter++;
										}
									);
									context.fetch_remove(other.id());

									return;
								}
							}

							if (ph.velocity.x > 0)
							{
								ph.velocity.x = 0;
								tr.position.x = _tr.position.x + _cld.offset.x - cld.size.x - cld.offset.x;
							}

							else if (ph.velocity.x < 0)
							{
								ph.velocity.x = 0;
								tr.position.x = _tr.position.x + _cld.offset.x + _cld.size.x - cld.offset.x;
							}
						}
					}
				}
			);

			ph.velocity.y -= ph.gravity * delta_time;

			tr.position.y += ph.velocity.y * delta_time;

			ph.on_ground = false;

			context.query_with_each
			(
				[&](entity& other, UUID& _uuid, transform2d& _tr, collider2d& _cld)
				{
					if (uuid != _uuid)
					{

						if (collider2d::is_colliding(tr, _tr, cld, _cld))
						{
							auto _tag = other.get_if_has<tag_component>();

							if (_tag)
							{
								if (_tag->value == COIN)
								{
									context.query
									(
										[](script_component& scr, tag_component& tag)
										{
											if (tag.value == COIN_COUNTER) scr.get_body<counter_script>().counter++;
										}
									);
									context.fetch_remove(other.id());

									return;
								}
							}

							if (ph.velocity.y < 0)
							{
								tr.position.y = _tr.position.y + _cld.size.y + _cld.offset.y - cld.offset.y;

								if (_tag)
								{
									if (_tag->value == PAD)
									{
										ph.velocity.y = 1000;
										ph.on_ground = false;
									}
									else
									{
										ph.velocity.y = 0;
										ph.on_ground = true;
									}
								}
								else
								{
									ph.velocity.y = 0;
									ph.on_ground = true;
								}

								if (auto _ph = other.get_if_has<physics2d>())
								{
									ph.velocity.x += _ph->force.x / _ph->mass;
								}
							}
							else if (ph.velocity.y > 0)
							{
								tr.position.y = _tr.position.y - cld.size.y + _cld.offset.y - cld.offset.y;
								ph.velocity.y = 0;
							}
						}
					}
				}
			);
		};

	};

	inline void player2_prefab(scene& scene, entity& self)
	{
		self.emplace<UUID>().id = self.id();
		self.emplace<tag_component>(PLAYER);
		self.emplace<transform2d>(vec2{ 200.0f, 200.0f });
		self.emplace<collider2d>(vec2{ 100, 100 }, vec2{ 0 , 0 });
		self.emplace<physics2d>(vec2{ 0,0 }, vec2{ 0,0 }, 2.0f, 1600.0f);

		anim_sprite2d_params params1 =
		{
			vec2{ 120.0f , 80.0f },
			vec2{ 360.0f * 3 , 240.0f * 3 },
			anim_test2,
			0.2f,
			0
		};

		anim_sprite2d_params params2 =
		{
			vec2{ 120.0f , 80.0f },
			vec2{ 360.0f * 3 , 240.0f * 3 },
			anim_test_22,
			0.2f,
			0
		};

		self.emplace<anim_sprite2d_bundle>(10)
			.put(0, renderer::textures["knight_idle_right"], params1)
			.put(1, renderer::textures["knight_idle_left"], params1)
			.put(2, renderer::textures["knight_run_right"], params1)
			.put(3, renderer::textures["knight_run_left"], params1)
			.put(4, renderer::textures["knight_turn_right"], params2)
			.put(5, renderer::textures["knight_turn_left"], params2)
			.put(6, renderer::textures["knight_jump_right"], params2)
			.put(7, renderer::textures["knight_jump_left"], params2)
			.put(8, renderer::textures["knight_idle_right"], params2)
			.put(9, renderer::textures["knight_idle_left"], params2);

		self.emplace<script_component>().bind<player2_script>(scene, self);

	}

}