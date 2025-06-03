#pragma once
#include <utility>
#include <memory>
#include <type_traits>

#include "../../debug/debug.h"

#define KW_DYN_ARRAY_DEFAULT_CAPACITY 128
#define KW_DYN_ARRAY_GROWTH_FACTOR 2

namespace kawa
{
	template<typename T>
	class dyn_array
	{
	public:
		inline dyn_array() noexcept
			: _data(static_cast<T*>(std::malloc(KW_DYN_ARRAY_DEFAULT_CAPACITY * sizeof(T))))
			, _occupied(0)
			, _capacity(KW_DYN_ARRAY_DEFAULT_CAPACITY)
		{}

		inline dyn_array(size_t size) noexcept
			: _data(static_cast<T*>(std::malloc(size * sizeof(T))))
			, _occupied(0)
			, _capacity(size)
		{}

		inline dyn_array(dyn_array&& other) noexcept
			: _data(other._data)
			, _occupied(other._occupied)
			, _capacity(other._capacity)
		{
			other._data = nullptr;
			other._occupied = 0;
			other._capacity = 0;
		}

		inline ~dyn_array() noexcept
		{
			for (size_t i = 0; i < _occupied; i++)
			{
				(_data + i)->~T();
			}

			_occupied = 0;

			if (_data)
			{
				std::free(_data);
				_data = nullptr;
			}
		}

		template<typename...Args>
		inline T& emplace_back(Args&&...args) noexcept
		{
			

			if (_occupied == _capacity)
			{
				KW_LOG("REALLOC");
				realloc_and_expand();
			}

			_occupied++;
			return *new(_data + _occupied - 1) T(std::forward<Args>(args)...);
		}

		inline void pop() noexcept
		{
			if (_occupied)
			{
				_occupied--;
				(_data + _occupied)->~T();
			}
		}

		[[nodiscard]] inline T& get(size_t id) noexcept
		{
			return _data[id];
		}

		inline void clear() noexcept
		{
			for (size_t i = 0; i < _occupied; i++)
			{
				(_data + i)->~T();
			}

			_occupied = 0;
		}

		inline size_t size() noexcept
		{
			return _occupied;
		}

		inline size_t capacity() noexcept
		{
			return _capacity;
		}


	public:
		[[nodiscard]] inline T& operator[](size_t id) noexcept
		{
			return get(id);
		}

		inline T* begin() noexcept
		{
			return _data;
		}

		inline T* end() noexcept
		{
			return _data + _occupied;
		}

	private:
		inline void realloc_and_expand() noexcept
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				_capacity *= KW_DYN_ARRAY_GROWTH_FACTOR;
				T* _new_buf = static_cast<T*>(std::malloc(_capacity * sizeof(T)));
				for (size_t i = 0; i < _occupied; i++)
				{
					new (_new_buf + i) T(std::move(_data[i]));
					(_data + i)->~T();
				}
				std::free(_data);
				_data = _new_buf;
				return;
			}
			else
			{
				_capacity *= KW_DYN_ARRAY_GROWTH_FACTOR;
				T* _new_buf = nullptr;
				while (!_new_buf)
				{
					_new_buf = static_cast<T*>(std::realloc(_data, _capacity * sizeof(T)));
				}
				_data = _new_buf;		
				return;
			}
		}

	private:
		T* _data;
		size_t _occupied;
		size_t _capacity;

	};
}
