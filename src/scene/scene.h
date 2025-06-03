#pragma once

#include "entity.h"

#include "../core/ecs/dynamic_registry.h"
#include "../core/data_structures/dyn_array.h"

//#include "../core/components/components.h"

#include "scene_base.h"    


namespace kawa
{
    class scene : public scene_base
    {
    public:
        scene()
            : _ctx(0xFFFF) 
        {
        }

        ~scene() = default;

    public:
            
        inline void fetch_remove(ecs::entity_id id) noexcept
        {
            _to_clear.emplace_back(id);
        }

        template<typename Script, typename...Args>
        inline void query(Script&& script, Args&&...args) noexcept
        {
            _ctx.query(std::forward<Script>(script), std::forward<Args>(args)...);
        }

        template<typename Script, typename...Args>
        inline void query_with(ecs::entity_id id, Script&& script, Args&&...args) noexcept
        {
            _ctx.query_with(id, std::forward<Script>(script), std::forward<Args>(args)...);
        }

        template<typename Script, typename...Args>
        inline void query_each(Script&& script, Args&&...args) noexcept
        {
            for (auto& [key, e] : _entity_container)
            {
                e.query(std::forward<Script>(script), std::forward<Args>(args)...);
            }
        }

        template<typename Script, typename...Args>
        inline void query_with_each(Script&& script, Args&&...args) noexcept
        {
            for (auto& [key, e] : _entity_container)
            {
                e.query_self(std::forward<Script>(script), std::forward<Args>(args)...);
            }
        }

        inline void cleanup() noexcept
        {
            for (auto e : _to_clear)
            {
                _entity_container.erase(e);
            }

            _to_clear.clear();
        }

        inline scene& prefab(void(prefab_func)(scene&)) noexcept
        {
            prefab_func(*this);

            return *this;
        }                     

        inline ecs::dynamic_registry& get_ctx()
        {
            return _ctx;
        }

        template<typename...EntityPrefabFn>
            requires (std::same_as<EntityPrefabFn, void(*)(entity&)> && ...)
        inline entity& entity_from_prefabs(EntityPrefabFn...prefabs) noexcept
        {
            auto e = entity(_ctx, *this);

            auto id = e.id();

            _entity_container.emplace(id, std::move(e));

            ((prefabs(_entity_container.at(id))),...);

            return _entity_container.at(id);
        }

        void serealize(const char* name);
        void deserealize(const char* filepath);

    public:
        ecs::dynamic_registry _ctx;
        dyn_array<uint64_t> _to_clear;

    private:
        std::unordered_map<uint64_t, entity> _entity_container;

        friend bindable<scene>;
    };

    template<>
    template<typename SceneScriptBody>
    void bindable<scene>::bind_impl(scene& _cnt)
    {
        _cnt._on_create_fn = [](void* body)
            {
                static_cast<SceneScriptBody*>(body)->on_create();
            };

        _cnt._on_update_fn = [](void* body, float delta_time)
            {
                static_cast<SceneScriptBody*>(body)->on_update(delta_time);
            };

        _cnt._on_render_fn = [](void* body)
            {
                static_cast<SceneScriptBody*>(body)->on_render();
            };
    }
}

