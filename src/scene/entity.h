#pragma once

//#include "../core/data_structures/dynamic_registry.h"
//#include "../core/data_structures/ssrutils.h"
#include "../core/ecs/dynamic_registry.h"

#include "../debug/debug.h"

namespace kawa
{
	class scene;

	class entity
	{
	public:
		inline entity(ecs::dynamic_registry& ctx, scene& s) noexcept
			: _ctx(ctx)
			, _scene_ctx(s)			
		{
			_id = _ctx.entity();

			if (_id == ecs::nullent)
			{
				KW_LOG_ERROR("bad entity");
			}		

		}

		inline entity(entity&& other) noexcept
			: _ctx(other._ctx)
			, _scene_ctx(other._scene_ctx)
			, _id(other._id) 
		{
			other._id = ecs::nullent;
		}

		inline ~entity() noexcept
		{
			if (_id != ecs::nullent)
			{
				_ctx.destroy(_id);
				KW_LOG("Destroyed entity:", _id);
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

		inline entity& apply_prefab(void(prefab)(entity&)) noexcept
		{
			prefab(*this);

			return *this;
		}

		template<typename T>
		[[nodiscard]] inline bool has() noexcept
		{
			return _ctx.has<T>(_id);
		}

		[[nodiscard]] inline ecs::dynamic_registry& get_ctx() noexcept
		{
			return _ctx;
		}

		[[nodiscard]] inline ecs::entity_id id() const noexcept
		{
			return _id;
		}

		[[nodiscard]] inline scene& get_scene()
		{
			return _scene_ctx;
		}

	public:
		ecs::dynamic_registry& _ctx;
		scene& _scene_ctx;
		ecs::entity_id _id;

	};

}
