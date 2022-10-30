#include "level.h"
#include "game.h"

using namespace cd;

Level::Level(
    bn::regular_bg_item _load_bg,
    const int *_int_grid,
    const Entity **_entities,
    bn::fixed _number_of_entities,
    bn::optional<bn::music_item> _music) : load_bg(_load_bg),
                                           int_grid(_int_grid),
                                           entities(_entities),
                                           number_of_entities(_number_of_entities),
                                           music(_music)
{
}

Level::~Level()
{
    reset();
}

void Level::tick(Game *game)
{
    hud->on_tick(game);
    bg->set_camera(game->get_camera());
    bool current_wave_finished = true;

    erase_if(
        waves,
        [=](Wave *wave)
        {
            if (current_wave == wave->get_wave_order())
            {
                wave->on_tick(game);

                if (wave->to_be_removed())
                {
                    delete wave;
                    return true;
                }
            }
            return false;
        });

    for (Wave *wave : waves)
    {
        if (current_wave == wave->get_wave_order())
        {
            current_wave_finished = false;
            break;
        }
    }

    for (Tower *tower : towers)
    {
        tower->on_tick(game);
    }

    for (Sheep *sheep : sheeps)
    {
        sheep->on_tick(game);
    }

    if (waves.empty())
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

void Level::stop_music()
{
    if (bn::music::playing())
    {
        bn::music::stop();
    }
}

void Level::init(bn::camera_ptr camera)
{
    stop_music();
    if (music.has_value())
    {
        music->play();
    }
    clear_waves();
    clear_towers();
    clear_sheeps();
    camera.set_position(0, 0);
    hud = HUD();

    bg = load_bg.create_bg(0, 0);

    bg->set_camera(camera);
    bg->set_position(0, 0);

    for (int i = 0; i < number_of_entities; ++i)
    {
        bn::fixed_point ldtk_coord_to_us = bn::fixed_point(
            entities[i]->get_position().x() - (bg->dimensions().width() / 2),
            entities[i]->get_position().y() - (bg->dimensions().height() / 2));

        switch (entities[i]->get_type())
        {
        case EntityType::Wave:
            waves.push_back(new Wave(
                entities[i]->get_id(),
                ldtk_coord_to_us, bg->camera().value(),
                entities[i]->get_number_1(),
                entities[i]->get_arr_points_1(),
                entities[i]->get_arr_points_1_size(),
                entities[i]->get_number_2(),
                entities[i]->get_number_3(),
                entities[i]->get_enemy_type()));
            break;
        case EntityType::Sheep:
            sheeps.push_back(new Sheep(
                entities[i]->get_id(),
                bg->camera().value(),
                ldtk_coord_to_us,
                entities[i]->get_arr_points_1(),
                entities[i]->get_arr_points_1_size()));
            break;
        default:
            log("cannot create unkown entity, im not god yet");
        }
    }
}

bn::vector<Wave *, 10> *Level::get_waves()
{
    return &waves;
}

bn::vector<Tower *, 20> *Level::get_towers()
{
    return &towers;
}

GridTileType Level::get_tile_type(int tile_index)
{
    return static_cast<GridTileType>(int_grid[tile_index]);
}

GridTileType Level::get_map_cell(bn::fixed x, bn::fixed y)
{
    bn::fixed ldtk_coord_x = (x + (bg->dimensions().width() / 2)).safe_division(16).right_shift_integer();
    bn::fixed ldtk_coord_y = (y + (bg->dimensions().height() / 2)).safe_division(16).right_shift_integer();

    bn::fixed ldtk_int_grid_row_size = bg->dimensions().width() / 16;
    bn::fixed ldtk_int_grid_col_size = bg->dimensions().height() / 16;

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

bool Level::add_tower(Tower *tower)
{
    if (towers.full())
    {
        log("level is full of towers");
        return false;
    }

    towers.push_back(tower);
    return true;
}

bool Level::is_won()
{
    return all_waves_finished;
}

void Level::reset()
{
    hud.reset();
    clear_waves();
    clear_towers();
    clear_sheeps();
    bg.reset();
    current_wave = 0;
    all_waves_finished = false;
    stop_music();
}

void Level::clear_towers()
{
    for (Tower *tower : towers)
    {
        delete tower;
    }
    towers.clear();
}

void Level::clear_sheeps()
{
    for (Sheep *sheep : sheeps)
    {
        delete sheep;
    }
    sheeps.clear();
}

void Level::clear_waves()
{
    for (Wave *wave : waves)
    {
        delete wave;
    }
    waves.clear();
}
