#pragma once

#include <unordered_map>
#include <typeindex>

#include "sentinel_set.h"
#include "ssrutils.h"

namespace ssr
{

	typedef uint64_t entity_id;
	typedef uint64_t storage_id;


	constexpr static entity_id nullent = 0;

	class dynamic_registry
	{
	public:

	public:
		inline dynamic_registry(size_t size) noexcept :
			_capacity(size),
			_entity_mask(reinterpret_cast<bool*>(std::calloc(size + 1, sizeof(bool))))
		{
		}

		inline dynamic_registry(dynamic_registry&& other) noexcept :
			_data(std::move(other._data)),
			_entity_mask(other._entity_mask),
			_capacity(other._capacity)
		{
			other._capacity = 0;
			other._entity_mask = nullptr;
		}

		inline ~dynamic_registry()
		{
			std::free(_entity_mask);
		}

	public:
		[[nodiscard]] inline entity_id entity() noexcept
		{
			for (size_t i = 1; i < _capacity; i++)
			{
				bool& mask = _entity_mask[i];
				if (!mask)
				{
					mask = true;
					return i;
				}
			}
		}

		template<typename T, typename...Args>
		inline T& emplace(entity_id id, Args&&...args) noexcept
		{
			auto key = typeid(T).hash_code();

			if (_data.contains(key))
			{
				auto& set = _data.at(key);

				if (set.contains(id))
				{
					set.erase(id);
				}

				return set.emplace<T>(id, std::forward<Args>(args)...);
			}
			else
			{

				_data.emplace(key, std::move(sentinel_set::create<T>(_capacity)));

				return _data.at(key).emplace<T>(id, std::forward<Args>(args)...);
			}
		}

		template<typename T>
		[[nodiscard]] inline T& get(entity_id id) noexcept
		{
			return _data.at(typeid(T).hash_code()).get<T>(id);
		}

		template<typename T>
		[[nodiscard]] inline T* get_if_has(entity_id id) noexcept
		{

			auto key = typeid(T).hash_code();
			if (_data.contains(key))
			{
				return _data.at(key).get_if_exsist<T>(id);
			}
			return nullptr;
		}

		template<typename T>
		inline void erase(entity_id id) noexcept
		{
			auto key = typeid(T).hash_code();
			if (_data.contains(key))
			{
				auto& set = _data.at(key);

				set.erase(id);
			}
		}

		inline void destroy(entity_id id) noexcept
		{
			for (auto& [type_id, set] : _data)
			{
				set.erase(id);
			}
		}

		template<typename T>
		[[nodiscard]] inline bool has(entity_id id) noexcept
		{
			auto key = typeid(T).hash_code();
			if (_data.contains(key))
			{
				auto& set = _data.at(key);

				return set.contains(id);
			}

			return false;

		}

		template<typename Fn, typename...PArgs>
		inline void query(Fn&& func, PArgs&&...pargs) noexcept
		{
			using args_tuple = function_traits<Fn>::template args_type_tuple;

			if constexpr (sizeof...(PArgs) == std::tuple_size_v<args_tuple>)
			{
				std::forward<Fn>(func)(std::forward<PArgs>(pargs)...);
			}
			else
			{
				query_helper<args_tuple>(std::make_index_sequence<std::tuple_size_v<args_tuple> -sizeof...(PArgs) - 1> {}, std::forward<Fn>(func), std::forward<PArgs>(pargs)...);
			}
		}

		template<typename Fn, typename...PArgs>
		inline void query_with(entity_id id, Fn&& func, PArgs&&...pargs) noexcept
		{
			using args_tuple = function_traits<Fn>::template args_type_tuple;

			if constexpr (sizeof...(PArgs) == std::tuple_size_v<args_tuple>)
			{
				std::forward<Fn>(func)(std::forward<PArgs>(pargs)...);
			}
			else
			{
				query_with_helper<args_tuple>(std::make_index_sequence<std::tuple_size_v<args_tuple> -sizeof...(PArgs) - 1> {}, id, std::forward<Fn>(func), std::forward<PArgs>(pargs)...);
			}
		}

	private:
		template<typename Tuple, typename Fn, typename...PArgs, size_t...I>
		inline void query_helper(std::index_sequence<I...>, Fn&& func, PArgs&&...pargs) noexcept
		{
			using last = std::tuple_element_t<std::tuple_size_v<Tuple> -1, Tuple>;

			uint64_t last_key = typeid(last).hash_code();

			uint64_t keys[std::tuple_size_v<Tuple>] = { typeid(std::tuple_element_t<I + sizeof...(PArgs), Tuple>).hash_code() ... };



			if (_data.contains(last_key) && (_data.contains(keys[I]) && ...))
			{
				auto& set = _data.at(last_key);

				for (size_t i = 1; i < set._occupied; i++)
				{
					size_t val = set._r_connector[i];

					if ((_data.at(keys[I]).contains(val) && ...))
					{
						std::forward<Fn>(func)(std::forward<PArgs>(pargs)..., _data.at(keys[I]).get<std::tuple_element_t<I + sizeof...(PArgs), Tuple>>(val)..., set.get<last>(val));
					}
				}
			}
		}

		template<typename Tuple, typename Fn, typename...PArgs, size_t...I>
		inline void query_with_helper(std::index_sequence<I...>, entity_id id, Fn&& func, PArgs&&...pargs) noexcept
		{
			using last = std::tuple_element_t<std::tuple_size_v<Tuple> -1, Tuple>;

			uint64_t last_key = typeid(last).hash_code();

			uint64_t keys[std::tuple_size_v<Tuple>] = { typeid(std::tuple_element_t<I + sizeof...(PArgs), Tuple>).hash_code() ... };

			if (_data.contains(last_key) && (_data.contains(keys[I]) && ...))
			{
				auto& set = _data.at(last_key);

				if (_data.at(last_key).contains(id) && (_data.at(keys[I]).contains(id) && ...))
				{
					std::forward<Fn>(func)(std::forward<PArgs>(pargs)..., _data.at(keys[I]).get<std::tuple_element_t<I + sizeof...(PArgs), Tuple>>(id)..., set.get<last>(id));
				}
			}
		}

	private:
		size_t _capacity;
		bool* _entity_mask;

	private:
		std::unordered_map<storage_id, sentinel_set> _data;

	};

}

