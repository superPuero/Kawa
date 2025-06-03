#include "scene.h"
#include "entity.h"

#include "../core/components/components.h"
#include "../core/serealizer.h"
#include "../core/deserealizer.h"

namespace kawa
{
    void scene::serealize(const char* name)
    {
        serealizer s(name);

        s.append(name);

        s.process(*this);

        query_with_each
        (
            [&](entity& e)
            {
                s.append("entity");
                s.process_if_exsist(e.get_if_has<UUID>());
                s.process_if_exsist(e.get_if_has<transform>());
                s.process_if_exsist(e.get_if_has<collider2d>());
                s.process_if_exsist(e.get_if_has<sprite2d>());
                s.process_if_exsist(e.get_if_has<sprite2d_bundle>());
                s.process_if_exsist(e.get_if_has<proj_camera_component>());
                s.process_if_exsist(e.get_if_has<ortho_camera_component>());
                s.process_if_exsist(e.get_if_has<physics2d>());

                s.process_if_exsist(e.get_if_has<script_component>());
                s.process_if_exsist(e.get_if_has<button_component>());
            }
        );
    };

    void scene::deserealize(const char* filepath)
    {
        deserealizer d(filepath);

        entity* e = nullptr;

        while (d.next())
        {
            if (d.is_current("scene"))
            {
                d.scene_parese(*this);
            }

            if (d.is_current("entity"))
            {
                e = new entity(_ctx, *this);

                auto id = e->emplace<UUID>().id;

                _entity_container.emplace(id, std::move(*e));

                delete e;

                e = &_entity_container.at(id);
            }

            else if (d.is_current("ortho_camera_component"))
            {
                d.entity_parse<ortho_camera_component>(*e);
            }

            else if (d.is_current("collider2d"))
            {
                d.entity_parse<collider2d>(*e);
            }

            else if (d.is_current("sprite2d"))
            {
                d.entity_parse<sprite2d>(*e);
            }

            else if (d.is_current("sprite2d_bundlex"))
            {
                d.entity_parse<sprite2d_bundle>(*e);
            }

            else if (d.is_current("proj_camera_component"))
            {
                d.entity_parse<proj_camera_component>(*e);
            }

            else if (d.is_current("transform"))
            {
                d.entity_parse<transform>(*e);
            }

            else if (d.is_current("physics2d"))
            {
                d.entity_parse<physics2d>(*e);
            }

            else if (d.is_current("script_component"))
            {
                d.entity_parse<script_component>(*e);
            }

            else if (d.is_current("button_component"))
            {
                d.entity_parse<button_component>(*e);
            }

        }
    }

}