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
}
