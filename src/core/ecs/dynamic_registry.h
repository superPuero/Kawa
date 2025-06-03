#pragma once
#include <iostream>
#include "../../debug/debug.h"

#include "smart_storage.h"

#include "../../meta/function_traits.h"
#include "../../meta/utils.h"

#define KW_MAX_STORAGE_COUNT 255

namespace kawa
{
	namespace ecs
	{
		typedef size_t entity_id;
		typedef size_t storage_id;

		constexpr inline entity_id nullent = -1;

		class dynamic_registry
		{

		public:
			inline dynamic_registry(size_t size) noexcept
			{
				void* block = std::calloc(1, size * (sizeof(smart_storage_base) + sizeof(bool) + sizeof(bool) + sizeof(size_t)));
				_storage = static_cast<smart_storage_base*>(block);
				_storage_mask = reinterpret_cast<bool*>(_storage + size);
				_entity_mask = reinterpret_cast<bool*>(_storage_mask + size);
				_entity_free_queue = reinterpret_cast<size_t*>(_entity_mask + size);
				_capacity = size;
				_entity_free_next = 0;
				_entity_free_queue_occupied = 0;
			}

		public:
			[[nodiscard]] inline entity_id entity() noexcept
			{
				if (_entity_free_queue_occupied)
				{
					_entity_free_queue_occupied--;

					entity_id id = _entity_free_queue[_entity_free_queue_occupied];

					_entity_mask[id] = true;

					return id;
				}

				entity_id id = _entity_free_next++;

				_entity_mask[id] = true;

				return id;
			}

			template<typename T, typename...Args>
			inline T& emplace(entity_id e, Args&&...args) noexcept
			{
				KW_ASSERT((e < _capacity));
				KW_ASSERT(_entity_mask[e]);

				storage_id s = get_storage_id<T>();
				bool& storage_cell = _storage_mask[s];

				if (!storage_cell)
				{
					storage_cell = true;
					new(_storage + s) smart_storage<T>(_capacity);
				}

				return _storage[s].emplace<T>(e, std::forward<Args>(args)...);
			}


			template<typename T>
			inline void remove(entity_id e) noexcept
			{
				KW_ASSERT((e < _capacity));
				KW_ASSERT(_entity_mask[e]);
				KW_ASSERT(_storage_mask[get_storage_id<T>()]);

				storage_id i = get_storage_id<T>();

				if (_entity_mask[e])
				{
					if (_storage_mask[i])
					{
						_storage[i].erase(e);
					}
				}
			}


			inline void destroy(entity_id e) noexcept
			{
				KW_ASSERT((e < _capacity));

				bool& entity_cell = _entity_mask[e];

				if (entity_cell)
				{
					for (size_t i = 0; i < _storage_id_counter; i++)
					{
						if (_storage_mask[i])
						{
							_storage[i].erase(e);
						}
					}

					_entity_free_queue[_entity_free_queue_occupied] = e;
					_entity_free_queue_occupied++;

					entity_cell = false;
				}
			}

			template<typename T>
			inline void erase(entity_id e) noexcept
			{
				storage_id i = get_storage_id<T>();

				if (_entity_mask[e])
				{
					if (_storage_mask[i])
					{
						_storage[i].erase(e);
					}
				}
			}

			template<typename T>
			[[nodiscard]] inline T& get(entity_id e) noexcept
			{
				KW_ASSERT((e < _capacity));
				KW_ASSERT(_entity_mask[e]);
				KW_ASSERT(_storage_mask[get_storage_id<T>()]);

				return _storage[get_storage_id<T>()].get<T>(e);
			}


			template<typename T>
			[[nodiscard]] inline T* get_if_has(entity_id e) noexcept
			{
				KW_ASSERT((e < _capacity));
				KW_ASSERT(_entity_mask[e]);

				if (_storage_mask[get_storage_id<T>()])
				{
					return _storage[get_storage_id<T>()].get_if_exsists<T>(e)	;
				}

				return nullptr;
			}

			template<typename T>
			[[nodiscard]] inline bool has(entity_id e) noexcept
			{
				KW_ASSERT((e < _capacity));
				KW_ASSERT(_entity_mask[e]);

				if (_storage_mask[get_storage_id<T>()])
				{
					return _storage[get_storage_id<T>()].contains<T>(e);
				}
			}

			template<typename Fn, typename...PArgs>
			inline void query(Fn&& fn, PArgs&&...pargs)
			{
				using args_tuple = meta::function_traits<Fn>::args_type_tuple;

				if constexpr (std::tuple_size_v<args_tuple> == sizeof...(PArgs))
				{
					std::forward<Fn>(fn)(std::forward<PArgs>(pargs)...);
				}
				else
				{
					using q_args_tuple = meta::slice_tuple<args_tuple>::template at<sizeof...(PArgs)>;

					query_impl<q_args_tuple, std::tuple_element_t<std::tuple_size_v<q_args_tuple> -1, q_args_tuple>>(std::make_index_sequence<std::tuple_size_v<q_args_tuple> -1>{}, std::forward<Fn>(fn), std::forward<PArgs>(pargs)...);
				}

			}

			template<typename Fn, typename...PArgs>
			inline void query_with(entity_id e, Fn&& fn, PArgs&&...pargs)
			{
				using args_tuple = meta::function_traits<Fn>::args_type_tuple;

				if constexpr (std::tuple_size_v<args_tuple> == sizeof...(PArgs))
				{
					std::forward<Fn>(fn)(std::forward<PArgs>(pargs)...);
				}
				else
				{
					using q_args_tuple = meta::slice_tuple<args_tuple>::template at<sizeof...(PArgs)>;

					query_with_impl<q_args_tuple, std::tuple_element_t<std::tuple_size_v<q_args_tuple> -1, q_args_tuple>>(std::make_index_sequence<std::tuple_size_v<q_args_tuple> -1>{}, e, std::forward<Fn>(fn), std::forward<PArgs>(pargs)...);
				}
			}

		private:
			template<typename Tuple, typename Last, typename Fn, typename...PArgs, size_t...I>
			inline void query_impl(std::index_sequence<I...>, Fn&& fn, PArgs&&...pargs) noexcept
			{
				storage_id storage_keys[sizeof...(I) + 1] = { get_storage_id<std::tuple_element_t<I, Tuple>>()..., get_storage_id<Last>() };

				if (_storage_mask[storage_keys[sizeof...(I)]])
				{
					if ((_storage_mask[storage_keys[I]] && ...))
					{
						smart_storage_base* storages[sizeof...(I) + 1] = { &_storage[storage_keys[I]]..., &_storage[storage_keys[sizeof...(I)]] };
						smart_storage_base& last = *storages[sizeof...(I)];

						for (size_t i = 0; i < last._entries_occupied; i++)
						{
							entity_id e = last._entries[i];

							if ((storages[I]->contains(e) && ...))
							{
								std::forward<Fn>(fn)(std::forward<PArgs>(pargs)..., storages[I]->get<std::tuple_element_t<I, Tuple>>(e)..., last.get<Last>(e));
							}
						}
					}
				}
			}

			template<typename Tuple, typename Last, typename Fn, typename...PArgs, size_t...I>
			inline void query_with_impl(std::index_sequence<I...>, entity_id e, Fn&& fn, PArgs&&...pargs) noexcept
			{
				storage_id storage_keys[sizeof...(I) + 1] = { get_storage_id<std::tuple_element_t<I, Tuple>>()..., get_storage_id<Last>() };

				if (_storage_mask[storage_keys[sizeof...(I)]])
				{
					if ((_storage_mask[storage_keys[I]] && ...))
					{
						smart_storage_base* storages[sizeof...(I) + 1] = { &_storage[storage_keys[I]]..., &_storage[storage_keys[sizeof...(I)]] };
						smart_storage_base& last = *storages[sizeof...(I)];

						if (last.contains(e))
						{
							if ((storages[I]->contains(e) && ...))
							{
								std::forward<Fn>(fn)(std::forward<PArgs>(pargs)..., storages[I]->get<std::tuple_element_t<I, Tuple>>(e)..., last.get<Last>(e));
							}
						}
					}
				}
			}


		private:
			template<typename T>
			static inline storage_id get_storage_id()
			{
				static storage_id id = _storage_id_counter++;
				return id;
			}

		private:
			smart_storage_base* _storage;
			bool* _storage_mask;
			bool* _entity_mask;

			size_t* _entity_free_queue;
			size_t _entity_free_queue_occupied;

			size_t _entity_free_next;
			size_t _capacity;

		private:
			static inline storage_id _storage_id_counter = 0;

		};
	}
}
