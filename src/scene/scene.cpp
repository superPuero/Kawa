#include "scene.h"
#include "entity.h"

#include "../core/components.h"
#include "../core/serealizer.h"
#include "../core/deserealizer.h"

namespace kawa
{
    void scene::serealize(const char* name) 
    {
        serealizer s(name);

        s.append(name);

        query_with_each
        (
            [&](entity& e)
            {
                s.append("entity");
                s.process_if_exsist(e.get_if_has<UUID>());
                s.process_if_exsist(e.get_if_has<transform2d>());
                s.process_if_exsist(e.get_if_has<collider2d>());
                s.process_if_exsist(e.get_if_has<sprite2d>());
                s.process_if_exsist(e.get_if_has<sprite2d_bundle>());
                s.process_if_exsist(e.get_if_has<camera_component>());
                s.process_if_exsist(e.get_if_has<script_component>());
            }
        );
    };
    void scene::load(const char* filepath)
    {
        deserealizer d(filepath);

        entity* e = nullptr;

        while (d.next())
        {
            if (d.is_current("entity"))
            {
                e = new entity(_ctx);

                auto id = e->emplace<UUID>().id;

                _entity_container.emplace(id, std::move(*e));

                delete e;

                e = &_entity_container.at(id);
            }

            else if (d.is_current("transform2d"))
            {
                d.parse(e->emplace<transform2d>());
            }

            else if (d.is_current("camera_component"))
            {
                d.parse(e->emplace<camera_component>());
            }

            else if (d.is_current("collider2d"))
            {
                d.parse(e->emplace<collider2d>());
            }

            else if (d.is_current("sprite2d"))
            {
                d.parse(e->emplace<sprite2d>());
            }

            else if (d.is_current("sprite2d_bundle"))
            {
                d.parse(e->emplace<sprite2d_bundle>());
            }

            else if (d.is_current("script_component"))
            {
                d.parse(e->emplace<script_component>(), *this, *e);
            }
        }
    }

}