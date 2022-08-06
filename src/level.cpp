#include "level.h"

using namespace cd;

Level::Level(
    bn::regular_bg_item _load_bg,
    const int *_int_grid,
    bn::fixed _world_x,
    bn::fixed _world_y,
    const Entity **_entities,
    bn::fixed _number_of_entities) : load_bg(_load_bg),
                                     int_grid(_int_grid),
                                     entities(_entities),
                                     number_of_entities(_number_of_entities)
{
    world_position = bn::fixed_point(_world_x, _world_y);
    start = bn::fixed_point(0, 0);
    end = bn::fixed_point(0, 0);
}

Level::~Level()
{
}

void Level::tick(bn::camera_ptr camera)
{
    bg.value().set_camera(camera);
}

void Level::init(bn::camera_ptr camera)
{
    camera.set_position(0, 0);

    bg = load_bg.create_bg(0, 0);

    bg.value().set_camera(camera);
    bg.value().set_position(0, 0);

    for (int i = 0; i < number_of_entities; ++i)
    {
        bn::fixed_point ldtk_coord_to_us = bn::fixed_point(
            entities[i]->get_position().x() - (bg.value().dimensions().width() / 2),
            entities[i]->get_position().y() - (bg.value().dimensions().height() / 2));

        switch (entities[i]->get_type())
        {
        case EntityType::Spawn:
            start = ldtk_coord_to_us;
            break;
        case EntityType::End:
            end = ldtk_coord_to_us;
            break;
        default:
            BN_LOG("cannot create unkown entity, im not god yet");
        }
    }
}

bn::fixed_point Level::get_start()
{
    return start;
}
bn::fixed_point Level::get_end()
{
    return end;
}
