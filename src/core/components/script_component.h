#pragma once

#include "../../scene/entity.h"
#include "../bindable/bindable_base.h"

#define KW_SCRIPT_DECL(script_name) struct script_name : script_component_logic_base<script_name>
#define KW_SCRIPT_HEADER() using script_component_logic_base::script_component_logic_base

namespace kawa
{
	class serealizer;
	class deserealizer;
	class script_component;

	struct script_component_base : public bindable<script_component>
	{
		using on_update_fn_t	= void(*)(void*, float);
		using on_create_fn_t	= void(*)(void*);
		using serealize_fn_t	= void(*)(void*, serealizer&);
		using deserealize_fn_t	= void(*)(void*, deserealizer&);

	public:		
		script_component_base() = default;
		script_component_base(const script_component_base&) = delete;
		script_component_base(script_component_base&&) noexcept = delete;


		inline void on_create() noexcept { _on_create_fn(_poly_body); }
		inline void on_update(float delta_time) noexcept { _on_update_fn(_poly_body, delta_time); }
		inline void _serealize(serealizer& ser)	noexcept { _serealize_fn(_poly_body, ser); }
		inline void _deserealize(deserealizer& deser) noexcept { _deserealize_fn(_poly_body, deser); }

	protected:
		on_update_fn_t		_on_update_fn = [](void*, float) { KW_LOG("Invoking on_update() on script_body which does not impl it"); };
		on_create_fn_t		_on_create_fn = [](void*) { KW_LOG("Invoking on_create() on script_body which does not impl it"); };
		serealize_fn_t		_serealize_fn = [](void*, serealizer&) { KW_LOG("Invoking serealize() on script_body which does not impl it"); };
		deserealize_fn_t	_deserealize_fn = [](void*, deserealizer&) { KW_LOG("Invoking derealize() on script_body which does not impl it"); };

	};

	struct script_component	: public script_component_base
	{
	public:
		script_component(entity* e)
			: self(e)
		{}

		template<typename ScriptBody>
		void bind() 
		{
			bindable<script_component>::bind<ScriptBody>(*this);
		}

		entity* self;

		friend bindable<script_component>;
	};

	template<typename ScriptBody>
	struct script_component_logic_base : public bindable_body_base<script_component, ScriptBody>
	{
		script_component_logic_base(script_component& sc)
			: self(*sc.self)
			, context(sc.self->get_scene())
		{
		}

		scene& context;
		entity& self;
	};

	template<>
	template<typename ScriptBody>
	void bindable<script_component>::bind_impl(script_component& _cnt)
	{
		_cnt._on_update_fn = [](void* script_body, float delta_time)
			{
				static_cast<ScriptBody*>(script_body)->on_update(delta_time);
			};


		_cnt._on_create_fn = [](void* script_body)
			{
				static_cast<ScriptBody*>(script_body)->on_create();
			};

		_cnt._serealize_fn = [](void* script_body, serealizer& ser)
			{
				static_cast<ScriptBody*>(script_body)->_serealize(ser);
			};

		_cnt._deserealize_fn = [](void* script_body, deserealizer& deser)
			{
				static_cast<ScriptBody*>(script_body)->_deserealize(deser);
			};	
	}
}
