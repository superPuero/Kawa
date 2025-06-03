#pragma once

#include "../../debug/debug.h"
#include "bindable_registrar.h"

namespace kawa
{
	template<typename Container>
	class bindable
	{
		using poly_deleter_fn_t = void(*)(void*);

	public:
		bindable() = default;

		~bindable()
		{
			KW_ASSERT(_poly_body);
			_poly_deleter_fn(_poly_body);
		}

	public:
		template<typename T>
		static void bind(Container& _cnt) 
		{ 			
			_cnt._poly_hash = typeid(T).hash_code();

			_cnt._poly_body = new T(_cnt);

			_cnt._poly_deleter_fn = [](void* body)
				{
					delete static_cast<T*>(body);
				};

			bind_impl<T>(_cnt);
		};

		template<typename T>
		static void bind_impl(Container& _cnt) 
		{
			KW_LOG("Invoking bind_impl with no impl for", typeid(Container).name()); 
		};

	public:
		inline size_t get_poly_hash() noexcept { return _poly_hash; };

	protected:
		size_t				_poly_hash			= 0;
		void*				_poly_body			= nullptr;
		poly_deleter_fn_t	_poly_deleter_fn	= nullptr;

	};

	template<typename Container, typename BindBody>
	class bindable_body_base
	{	
	private:
		static inline bindable_registrar<Container> reg = bindable_registrar<Container>::template _register<BindBody>();

	};
}

