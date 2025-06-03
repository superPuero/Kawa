#pragma once

#include <functional>
#include <unordered_map>

namespace kawa
{
	template<typename DestinationBody>
	class bindable_registrar
	{
	public:
		inline bindable_registrar() = default;

		template<typename BindableBody>
		static inline bindable_registrar _register() noexcept
		{
			_bindable_factory.emplace
			(
				typeid(BindableBody).hash_code(),
				[&](DestinationBody* dest, void* arg)
				{
					dest->bind<BindableBody>();
				}
			);

			return bindable_registrar();
		}
	  
		static inline std::unordered_map<size_t, std::function<void(DestinationBody*, void*)>> _bindable_factory;
	};
}