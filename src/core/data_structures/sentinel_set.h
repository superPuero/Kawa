#pragma once
#include <memory>
#include <type_traits>
#include <iostream>

class sentinel_set
{
public:
	template<typename T>
	inline static sentinel_set create(size_t size)
	{
		void(*clear_func)(void*, bool*, size_t);

		void* data = std::calloc(size + 1, sizeof(T));

		clear_func = [](void* data, bool* mask, size_t size)
			{
				T* ptr = static_cast<T*>(data);

				for (size_t i = 0; i < size; i++)
				{
					if (mask[i])
					{
						(ptr + i)->~T();
					}
				}
			};

		return sentinel_set
		(
			data,
			size,
			sizeof(T),
			[](void* data, bool* mask, size_t* connector, size_t occupied, size_t idx)
			{
				if (mask[idx])
				{
					(static_cast<T*>(data) + idx)->~T();

					if constexpr (std::is_move_constructible_v<T>)
					{
						new(reinterpret_cast<T*>(data) + connector[idx]) T(std::move(reinterpret_cast<T*>(data)[occupied]));
					}
					else
					{
						std::memcpy(data + connector[idx], data + occupied, sizeof(T));
					}

				}
			},
			clear_func
		);

	}

	inline sentinel_set() noexcept
	{

	}

	inline sentinel_set(sentinel_set&& other) noexcept
		: _size(other._size),
		_data(other._data),
		_mask(other._mask),
		_connector(other._connector),
		_r_connector(other._r_connector),
		_occupied(other._occupied),
		_deleter_func(other._deleter_func),
		_clear_func(other._clear_func)
	{

		other._data = nullptr;
		other._mask = nullptr;
		other._deleter_func = nullptr;
		other._clear_func = nullptr;
		other._connector = nullptr;
		other._r_connector = nullptr;

	}

	~sentinel_set()
	{
		if (_clear_func)
		{
			_clear_func(_data, _mask, _size);
			_clear_func;
		}

		std::free(_data);
		std::free(_mask);
		std::free(_connector);
		std::free(_r_connector);
	}

public:
	template<typename T, typename...Args>
	inline T& emplace(size_t idx, Args&&...args) noexcept
	{
		_mask[idx] = true;

		_connector[idx] = _occupied;

		_r_connector[_occupied] = idx;

		new(static_cast<T*>(_data) + _occupied) T(std::forward<Args>(args)...);

		_occupied++;

		return *(static_cast<T*>(_data) + _occupied - 1);
	}

	template<typename T>
	inline T& get(size_t idx) noexcept
	{
		return (reinterpret_cast<T*>(_data))[_connector[idx]];
	}

	template<typename T>
	inline T* get_if_exsist(size_t idx) noexcept
	{

		if (_mask[idx])
		{
			return &get<T>(idx);
		}

		return nullptr;
	}

	inline void erase(size_t idx) noexcept
	{
		if (_mask[idx])
		{
			_occupied--;

			_deleter_func(_data, _mask, _connector, _occupied, idx);

			_connector[_r_connector[_occupied]] = _connector[idx];

			_r_connector[_connector[idx]] = _r_connector[_occupied];
			_r_connector[_occupied] = 0;

			_mask[idx] = false;
		}
	}

	[[nodiscard]] inline bool contains(size_t idx)
	{
		return _mask[idx];
	}

	template<typename T>
	void data()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << i << "->" << get<T>(_r_connector[i]) << '\n';
		}
	}


	void connector()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << _r_connector[i] << "->" << _connector[_r_connector[i]] << '\n';
		}
	}


	void r_connector()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << i << "->" << _r_connector[i] << '\n';
		}
	}

private:
	sentinel_set(void* data, size_t size, size_t offset, void(*deleter_func)(void*, bool*, size_t*, size_t, size_t), void(*clear_func)(void*, bool*, size_t)) :
		_data(data),
		_mask(reinterpret_cast<bool*>(std::calloc(size + 1, sizeof(bool)))),
		_connector(reinterpret_cast<size_t*>(std::calloc(size + 1, sizeof(size_t)))),
		_r_connector(reinterpret_cast<size_t*>(std::calloc(size + 1, sizeof(size_t)))),
		_size(size),
		_offset(offset),
		_occupied(1),
		_deleter_func(deleter_func),
		_clear_func(clear_func)
	{}


public:
	size_t _occupied;
	size_t* _connector;
	size_t* _r_connector;

private:
	void(*_deleter_func)(void*, bool*, size_t*, size_t, size_t);
	void(*_clear_func)(void*, bool*, size_t);

private:
	void* _data;
	bool* _mask;
	size_t _size;
	size_t _offset;
};

