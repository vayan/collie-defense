#include "level.h"

using namespace cd;

Level::Level(
    bn::regular_bg_item _load_bg,
    const int *_int_grid,
    const Entity **_entities,
    bn::fixed _number_of_entities) : load_bg(_load_bg),
                                     int_grid(_int_grid),
                                     entities(_entities),
                                     number_of_entities(_number_of_entities)
{
}

Level::~Level()
{
    paths.clear();
}

void Level::tick(bn::camera_ptr camera, Player *player)
{
    bg.value().set_camera(camera);
    bool current_wave_finished = true;

    for (Path &path : paths)
    {
        if (current_wave == path.get_wave_order())
        {
            current_wave_finished = false;
            path.on_tick(this, player);

            if (path.to_be_removed())
            {
                paths.erase(&path);
            }
        }
    }

    for (Tower &tower : towers)
    {
        tower.on_tick(this, player);
    }

    if (paths.empty())
    {
        log("current level is finished!");
        all_waves_finished = true;
    }
    else if (current_wave_finished)
    {
        current_wave += 1;
        log("moving to wave number", current_wave + 1); // 0 indexed
        current_wave_finished = false;
    }
}

void Level::init(bn::camera_ptr camera)
{
    paths.clear();
    towers.clear();
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
        case EntityType::Path:
            paths.emplace_back(
                ldtk_coord_to_us, bg.value().camera().value(),
                entities[i]->get_number_1(),
                entities[i]->get_arr_points_1(),
                entities[i]->get_arr_points_1_size(),
                entities[i]->get_number_2(),
                entities[i]->get_number_3());
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

GridTileType Level::get_tile_type(int tile_index)
{
    return static_cast<GridTileType>(int_grid[tile_index]);
}

GridTileType Level::get_map_cell(bn::fixed x, bn::fixed y)
{
    bn::fixed ldtk_coord_x = (x + (bg.value().dimensions().width() / 2)).safe_division(16).right_shift_integer();
    bn::fixed ldtk_coord_y = (y + (bg.value().dimensions().height() / 2)).safe_division(16).right_shift_integer();

    bn::fixed ldtk_int_grid_row_size = bg.value().dimensions().width() / 16;
    bn::fixed ldtk_int_grid_col_size = bg.value().dimensions().height() / 16;

    // if we're outside of the intgrid we consider it like empty cell
    if (ldtk_coord_x < 0 || ldtk_coord_x >= ldtk_int_grid_row_size || ldtk_coord_y < 0 || ldtk_coord_y >= ldtk_int_grid_col_size)
    {
        return GridTileType::empty;
    }

    bn::fixed ldtk_int_grid_index = ldtk_int_grid_row_size * ldtk_coord_y + ldtk_coord_x;

    return get_tile_type(ldtk_int_grid_index.integer());
}

bn::optional<bn::regular_bg_ptr> Level::get_bg()
{
    return bg;
}

void Level::add_tower(bn::fixed_point position, TowerType type)
{
    if (towers.full())
    {
        log("level is full of towers");
        return;
    }
    towers.emplace_back(type, bg.value().camera().value(), position);
}

bool Level::is_won()
{
    return all_waves_finished;
}

void Level::reset()
{
    paths.clear();
    towers.clear();
    bg.reset();
    current_wave = 0;
    all_waves_finished = false;
}
