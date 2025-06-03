#pragma once

#include <memory>

#include <iostream>

namespace kawa
{
namespace ecs
{

	class smart_storage_base
	{
	public:
		inline smart_storage_base() = default;

		inline ~smart_storage_base() noexcept
		{
			std::free(_storage);
		}

		template<typename T, typename...Args>
		inline T& emplace(size_t i, Args&&...args)
		{
			bool& cell = _mask[i];
			
			if (!cell)
			{
				_entries[_entries_occupied] = i;
				_r_entries[i] = _entries_occupied;

				_entries_occupied++;

				cell = true;
				return *(new(static_cast<T*>(_storage) + i) T(std::forward<Args>(args)...));
			}

			return *(static_cast<T*>(_storage) + i);
		}

		inline bool contains(size_t i) noexcept
		{
			return _mask[i];
		}

		template<typename T>
		inline T& get(size_t i) noexcept
		{
			return *(static_cast<T*>(_storage) + i);
		}

		template<typename T>
		inline T* get_if_exsists(size_t i) noexcept
		{
			if (_mask[i])
			{
				return static_cast<T*>(_storage) + i;
			}

			return nullptr;
		}

		inline void erase(size_t i)
		{
			bool& cell = _mask[i];
			if (cell)
			{
				_entries_occupied--;
				_entries[_r_entries[i]] = _entries[_entries_occupied];

				cell = false;
				_deleter(_storage, i);
			}
		}



	public:
		void* _storage;
		bool* _mask;
		size_t* _entries;
		size_t* _r_entries;
		size_t	_entries_occupied;

		void(*_deleter)(void*, size_t);

		size_t	_tag;
	};

	template<typename T>
	class smart_storage : public smart_storage_base
	{
	public:
		inline smart_storage(size_t size) noexcept
		{
			void* block = std::calloc(1, size * (sizeof(T) + sizeof(bool) + sizeof(size_t) + sizeof(size_t)));

			_storage = static_cast<void*>(block);
			_mask = reinterpret_cast<bool*>(reinterpret_cast<T*>(_storage) + size);
			_entries = reinterpret_cast<size_t*>(reinterpret_cast<bool*>(_mask) + size);
			_r_entries = reinterpret_cast<size_t*>(reinterpret_cast<size_t*>(_entries) + size);

			_deleter = [](void* storage, size_t i)
				{
					(static_cast<T*>(storage) + i)->~T();
				};

			_entries_occupied = 0;
		}
	};
}

}
