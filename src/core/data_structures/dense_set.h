#pragma once
#include <iostream>
#include <type_traits>
#include <memory>

template<size_t size, typename ValTy>
class dense_set
{
public:
	static constexpr size_t capacity = size;

	inline dense_set() noexcept :
		_data(reinterpret_cast<ValTy*>(std::calloc(size, sizeof(ValTy)))),
		_connector(new size_t[size]),
		_r_connector(new size_t[size]),
		_occupied(1)
	{}

	inline ~dense_set() noexcept
	{
		std::free(_data);
		delete[] _connector;
		delete[] _r_connector;
	}

public:
	inline ValTy& get(size_t idx) noexcept
	{
		return _data[_connector[idx]];
	}

	template<typename...Args>
	inline ValTy& emplace(size_t idx, Args&&...args)	noexcept
	{
		_connector[idx] = _occupied;

		_r_connector[_occupied] = idx;

		new(_data + _occupied) ValTy(std::forward<Args>(args)...);

		_occupied++;

		return _data[_occupied - 1];
	}

	inline void erase(size_t idx) noexcept
	{
		_occupied--;

		_data[_connector[idx]].~ValTy();

		if constexpr (std::is_move_constructible_v<ValTy>)
		{
			new(_data + _connector[idx]) ValTy(std::move(_data[_occupied]));
		}
		else
		{
			std::memcpy(_data + _connector[idx], _data + _occupied, sizeof(ValTy));
		}

		_connector[_r_connector[_occupied]] = _connector[idx];

		_r_connector[_connector[idx]] = _r_connector[_occupied];
		_r_connector[_occupied] = 0;

	}

	inline void data()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << i << "->" << _data[i] << '\n';
		}
	}

	inline void connector()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << _r_connector[i] << "->" << _connector[_r_connector[i]] << '\n';
		}
	}

	inline void r_connector()
	{
		for (size_t i = 1; i < _occupied; i++)
		{
			std::cout << i << "->" << _r_connector[i] << '\n';
		}
	}

	inline ValTy& operator[](size_t idx)
	{
		return get(idx);
	}

public:
	ValTy* _data;
	size_t* _connector;
	size_t* _r_connector;
	size_t _occupied;

private:

};