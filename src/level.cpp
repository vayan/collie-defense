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
}

Level::~Level()
{
    paths.clear();
}

void Level::tick(bn::camera_ptr camera)
{
    bg.value().set_camera(camera);

    for (Path &path : paths)
    {
        path.on_tick(this);
    }

    for (Tower &tower : towers)
    {
        tower.on_tick(this);
    }
}

void Level::init(bn::camera_ptr camera)
{
    camera.set_position(0, 0);

    bg = load_bg.create_bg(0, 0);

    bg.value().set_camera(camera);
    bg.value().set_position(0, 0);

    towers.emplace_back(camera, bn::fixed_point(-25, 0));
    towers.emplace_back(camera, bn::fixed_point(-70, -90));

    for (int i = 0; i < number_of_entities; ++i)
    {
        bn::fixed_point ldtk_coord_to_us = bn::fixed_point(
            entities[i]->get_position().x() - (bg.value().dimensions().width() / 2),
            entities[i]->get_position().y() - (bg.value().dimensions().height() / 2));

        switch (entities[i]->get_type())
        {
        case EntityType::Path:
            paths.emplace_back(ldtk_coord_to_us, bg.value().camera().value(), entities[i]->get_number_1(), entities[i]->get_arr_points_1(), entities[i]->get_arr_points_1_size());
            break;
        default:
            log("cannot create unkown entity, im not god yet");
        }
    }
}

bn::vector<Path, 10> *Level::get_paths()
{
    return &paths;
}

bn::vector<Tower, 10> *Level::get_towers()
{
    return &towers;
}
