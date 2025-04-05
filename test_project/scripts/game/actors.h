#pragma once

#include "../../kawa.h"

namespace game
{
	using namespace kawa;


	static inline vec2 anim_test[10] =
	{
		vec2{0,0}, vec2{1,0}, vec2{2,0}, vec2{3,0},
		vec2{4,0}, vec2{5,0}, vec2{6,0}, vec2{7,0},
		vec2{8,0}, vec2{9,0}
	};

	static inline vec2 anim_test_2[3] =
	{
		vec2{0,0}, vec2{1,0}, vec2{2,0}
	};

	struct counter_script : script_component::script_base
	{
		using script_base::script_base;

		text_component& tc = self.get<text_component>();
		uint32_t counter = 0;

		void on_create()
		{
		
		}

		void on_update(float delta_time)
		{
			tc.data = std::to_string(counter);
		}
	};

	struct counter_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<tag_component>(COIN_COUNTER);
			self.emplace<text_component>("", vec2{ 32, 64 });
			self.emplace<transform2d>(vec2{ 0, application::application_instance->_window.win_height - 128 });

			self.emplace<script_component>().bind<counter_script>(scene, self);
		}
	};



	struct coin_script : script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{
		
		}

		void on_update(float delta_time)
		{

		}
	};

	struct coin_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<transform2d>(vec2(std::rand() % application::application_instance->_window.win_width, std::rand() % application::application_instance->_window.win_height));
			self.emplace<collider2d>(vec2{ 32, 32 }, vec2{ 0, 0 });
			self.emplace<tag_component>(entity_tag::COIN);
		}
	};


	struct spam_script : script_component::script_base
	{ 
		using script_base::script_base;

		void on_create()
		{

		}

		void on_update(float delta_time)
		{

		}
	};

	struct spam_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<transform2d>(vec2(std::rand() % application::application_instance->_window.win_width, std::rand() % application::application_instance->_window.win_height));
			self.emplace<collider2d>(vec2{ 16,16 }, vec2{ 0,0 });

			self.emplace<script_component>().bind<spam_script>(scene, self);

		}
	};



	struct text_script : script_component::script_base
	{
		using script_base::script_base;

		text_component& tc = self.get<text_component>();
		float timer = 0;


		void on_create()
		{
			tc.data = "fps";
		}

		void on_update(float delta_time)
		{
			if (timer > 0.5)
			{
				timer = 0;
				tc.data = std::to_string((int)(1.0f/delta_time));
			}
			else 				   
			{
				timer += delta_time;
			}
		}
	};

	struct text_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<transform2d>(vec2{ 0, application::application_instance->_window.win_height - 64 });
			self.emplace<text_component>("", vec2{ 32, 64 });
			self.emplace<script_component>().bind<text_script>(scene, self);
		}
	};

	inline void calculate(scene& scene, entity& self, float delta_time, UUID& uuid, transform2d& tr, physics2d& ph, collider2d& cld, anim_sprite2d_bundle& asb)
	{
		ph.velocity.x -= ph.mass * delta_time;

		tr.position.x += ph.velocity.x * delta_time;

		scene.query_with_each
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
								scene.query
								(
									[](script_component& scr, tag_component& tag)
									{
										if (tag.value == COIN_COUNTER) scr.get_body<counter_script>().counter++;
									}
								);
								scene.fetch_remove(other.id());
																					
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

		scene.query_with_each
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
								scene.query
								(
									[](script_component& scr, tag_component& tag) 
									{ 
										if (tag.value == COIN_COUNTER) scr.get_body<counter_script>().counter++ ; 
									}
								);
								scene.fetch_remove(other.id());

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

	struct player1_script : script_component::script_base
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

			if (application::application_instance->_window.is_key_down(GLFW_KEY_LEFT))
			{
				ph.force.x -= 50;
			}

			if (application::application_instance->_window.is_key_down(GLFW_KEY_RIGHT))
			{
				ph.force.x += 50;			
			}		
			if (application::application_instance->_window.is_key_down(GLFW_KEY_UP) && ph.on_ground)
			{
				ph.force.y = 1500;
				ph.on_ground = false;
			}

			if (application::application_instance->_window.is_key_down(GLFW_KEY_1))
			{
				application::application_instance->set_scene(0);
			}


			if (application::application_instance->_window.is_key_down(GLFW_KEY_0))
			{
				application::application_instance->set_scene(0);
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

			if (asb.time / 0.1f >= 1)
			{
				asb.next();
				asb.time = 0;
			}
			asb.time += delta_time;

			calculate(context, self, delta_time, uuid, tr, ph, cld, asb);
		}
	};

	struct player1_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			self.emplace<UUID>().id = self.id();
			self.emplace<tag_component>(PLAYER);
			self.emplace<transform2d>(vec2{ 200.0f, 200.0f });
			self.emplace<collider2d>(vec2{ 16, 16 }, vec2{ 120.0f * 3 / 2.0f - 30, 0.0f });
			self.emplace<physics2d>(vec2{ 0,0 }, vec2{ 0,0 }, 2.0f, 1600.0f);

			anim_sprite2d_params params1 =
			{
				vec2{ 120.0f , 80.0f },
				vec2{ 120.0f * 3, 80.0f * 3},
				anim_test,
				0.2f,
				0
			};

			anim_sprite2d_params params2 =
			{
				vec2{ 120.0f , 80.0f },
				vec2{ 120.0f , 80.0f },
				anim_test_2,
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

			self.emplace<script_component>().bind<player1_script>(scene, self);
		}
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

			calculate(context, self, delta_time, uuid, tr, ph, cld, asb);
		}
	};

	struct player2_prefab
	{
		static void prefab(scene& scene, entity& self)
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
				anim_test,
				0.2f,
				0
			};

			anim_sprite2d_params params2 =
			{
				vec2{ 120.0f , 80.0f },
				vec2{ 360.0f * 3 , 240.0f * 3 },
				anim_test_2,
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

	};


	struct block_script	: script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
		
		}
	};


	struct block_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			vec2 idxs[1][1] = { {vec2{0, 15}} };
			self.emplace<UUID>().id = self.id();
			self.emplace<tag_component>(PAD);
			self.emplace<transform2d>(vec2{ 1000.0f, 200.0f });
			self.emplace<collider2d>(vec2{ 200,  200 }, vec2{ 0 , 0 });

			self.emplace<script_component>().bind<block_script>(scene, self);

		}
	};



	struct block2_script :script_component::script_base
	{
		using script_base::script_base;

		void on_create()
		{

		};

		void on_update(float delta_time)
		{
		
		}
	};


	struct block2_prefab
	{
		static void prefab(scene& scene, entity& self)
		{
			vec2 idxs[10][1] = { {vec2{0, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{1, 19}},{vec2{2, 19}},{vec2{3, 19}} };
			self.emplace<UUID>().id = self.id();
			self.emplace<tag_component>(BLOCK);
			self.emplace<transform2d>(vec2{ 100.0f, 100.0f });
			self.emplace<collider2d>(vec2{ 1200,  25 }, vec2{ 0 , 0 });

			self.emplace<tile_map2d>(renderer::textures["tiles"], glm::vec2{ 20.0f, 20.0f }, glm::vec2{ 40.0f, 40.0f }, idxs);

			self.emplace<script_component>().bind<block2_script>(scene, self);
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
			self.emplace<transform3d>(vec3{ 0,0,-1 }, vec3{ glm::radians(-90.0f),0,0 });
			self.emplace<physics3d>(vec3{ 0,0,0 });

			self.get<camera_component>().proj = glm::ortho(0.0f, (float)application::application_instance->_window.win_width, 0.0f, (float)application::application_instance->_window.win_height, 0.01f, 1000.0f);

			self.emplace<script_component>().bind<ortho_camera_script>(scene, self);
		}
	};
}