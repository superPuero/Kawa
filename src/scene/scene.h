#pragma once

#include "entity.h"

#include "../core/data_structures/dynamic_registry.h"
#include "../core/data_structures/ssrutils.h"

#include "../core/components.h"

#include "scene_logic_body.h"    

namespace kawa
{
    class scene
    {
    public:

        scene()
            : _ctx(1000000)
        {

        }

        ~scene()
        {

        }

    public:
        void on_update(float delta_time) noexcept
        {
            on_update_func(_scene_logic_body, delta_time);
        }
        void on_create() noexcept
        {
            on_create_func(_scene_logic_body);
        }
        void on_render() noexcept
        {
            on_render_func(_scene_logic_body);
        }

        inline void fetch_remove(ssr::entity_id id) noexcept
        {
            _to_clear.push_back(id);
        }

        template<typename Script, typename...Args>
        inline void query(Script&& script, Args&&...args) noexcept
        {
            _ctx.query(std::forward<Script>(script), std::forward<Args>(args)...);
        }

        template<typename Script, typename...Args>
        inline void query_with(ssr::entity_id id, Script&& script, Args&&...args) noexcept
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

        inline void prefab(void(prefab_func)(scene&)) noexcept
        {
            prefab_func(*this);
        }                     

        inline ssr::dynamic_registry& get_ctx()
        {
            return _ctx;
        }

        inline void entity_from_prefab(void(prefab_func)(scene&, entity&)) noexcept
        {
            auto e = entity(_ctx);

            auto id = e.id();

            _entity_container.emplace(id, std::move(e));

            prefab_func(*this, _entity_container.at(id));
        }

        template<typename SceneScriptBody>
        inline void bind()
        {
            _scene_logic_body = new SceneScriptBody{ *this };

            _logic_body_deleter = [](void* script)
                {
                    delete static_cast<SceneScriptBody*>(script);
                };

            on_create_func = [](void* script)
                {
                    static_cast<SceneScriptBody*>(script)->on_create();
                };

            on_update_func = [](void* script, float delta_time)
                {
                    static_cast<SceneScriptBody*>(script)->on_update(delta_time);
                };

            on_render_func = [](void* script)
                {
                    static_cast<SceneScriptBody*>(script)->on_render();
                };

            on_create_func(_scene_logic_body);

        }

        void serealize(const char* name);

        void load(const char* filepath);

    public:
        ssr::dynamic_registry _ctx;
        std::vector<uint64_t> _to_clear;

    private:
        std::unordered_map<uint64_t, entity> _entity_container;

        void* _scene_logic_body;

        void(*on_update_func)(void*, float);
        void(*on_create_func)(void*);
        void(*on_render_func)(void*);

        void(*_logic_body_deleter)(void*);
    };
}

