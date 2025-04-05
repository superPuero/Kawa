#pragma once

#include "../core/data_structures/dynamic_registry.h"
#include "../core/data_structures/ssrutils.h"

namespace kawa
{
	class entity
	{
	public:
		inline entity(ssr::dynamic_registry& s) noexcept
			: _ctx(s)
		{
			_id = _ctx.entity();

			if (_id == ssr::nullent)
			{
				std::cout << "bad entity" << '\n';
			}		

		}

		inline entity(entity&& other) noexcept
			: _ctx(other._ctx)
			, _id(other._id)
		{
			other._id = ssr::nullent;
		}

		inline ~entity() noexcept
		{
			if (_id != ssr::nullent)
			{
				_ctx.destroy(_id);
				std::cout << "dest" << _id << '\n';
			}
		}

	public:

		template<typename Script, typename...Args>
		inline void query(Script&& script, Args&&...args) noexcept
		{
			_ctx.query_with(_id, std::forward<Script>(script), std::forward<Args>(args)...);
		}

		template<typename Script, typename...Args>
		inline void query_self(Script&& script, Args&&...args) noexcept
		{
			_ctx.query_with(_id, std::forward<Script>(script), *this, std::forward<Args>(args)...);
		}

		template<typename T, typename...Args>
		inline T& emplace(Args&&...args) noexcept
		{
			return _ctx.emplace<T>(_id, std::forward<Args>(args)...);
		}

		template<typename T>
		[[nodiscard]] inline T& get() noexcept
		{
			return _ctx.get<T>(_id);
		}

		template<typename T>
		inline void remove() noexcept
		{
			return _ctx.erase<T>(_id);
		}

		template<typename T>
		[[nodiscard]] inline T* get_if_has() noexcept
		{
			return _ctx.get_if_has<T>(_id);
		}

		template<typename T>
		[[nodiscard]] inline bool has() noexcept
		{
			return _ctx.has<T>(_id);
		}

		[[nodiscard]] inline ssr::dynamic_registry& get_ctx() noexcept
		{
			return _ctx;
		}

		[[nodiscard]] inline ssr::entity_id id() const noexcept
		{
			return _id;
		}

	public:
		ssr::dynamic_registry& _ctx;

		ssr::entity_id _id;

	};

}
