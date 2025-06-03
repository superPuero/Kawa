#pragma once
#include "../core/bindable/bindable_base.h"

#define KW_SCENE_LOGIC_DECL(name)struct name : scene_logic_base<name>
#define KW_SCENE_LOGIC_HEADER() using scene_logic_base::scene_logic_base

namespace kawa
{
	template<typename SceneLogic>
	class scene_logic_base : public bindable_body_base<scene, SceneLogic>
	{
	public:
		scene_logic_base(scene& s)
			: self(s)
		{
		}

		scene& self;
	};

	class scene_base : public bindable<scene>
	{
		using on_create_fn_t = void(*)(void*);
		using on_update_fn_t = void(*)(void*, float);
		using on_render_fn_t = void(*)(void*);

	public:
		scene_base() = default;
		scene_base(const scene_base&) = delete;
		scene_base(scene_base&&) noexcept = delete;
		
	public:
		void on_create() noexcept					{ _on_create_fn(_poly_body); }
		void on_update(float delta_time) noexcept	{ _on_update_fn(_poly_body, delta_time); }
		void on_render() noexcept					{ _on_render_fn(_poly_body); }

	public:
		template<typename SceneScriptBody>
		inline void bind(void* = nullptr)
		{
			bindable<scene>::bind<SceneScriptBody>(*(scene*)(this));
		}

	protected:
		on_create_fn_t _on_create_fn = [](void*)			{ KW_LOG_WARNING("Executing not implemented on_create scene logic method"); };
		on_update_fn_t _on_update_fn = [](void*, float)		{ KW_LOG_WARNING("Executing not implemented on_update scene logic method"); };
		on_render_fn_t _on_render_fn = [](void*)			{ KW_LOG_WARNING("Executing not implemented on_render scene logic method"); };

	};								   
}