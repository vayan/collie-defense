#include "cursor.h"

using namespace cd;

Cursor::Cursor()
{
    sprite = bn::sprite_items::cursor.create_sprite(0, 0);
}

Cursor::~Cursor()
{
}

void Cursor::enable()
{
    targeting_buildable_grid = true;
    // no idea if set_item is fast... let's see later
    sprite.value()
        .set_item(bn::sprite_items::cursor, 0);
}

void Cursor::disable()
{
    targeting_buildable_grid = false;
    sprite.value()
        .set_item(bn::sprite_items::cursor, 1);
}

void Cursor::on_tick(Level *level)
{
    bn::fixed_point camera_position = level->get_bg().value().camera().value().position();
    GridTileType grid = level->get_map_cell(
        camera_position.x(),
        camera_position.y());

    if (grid == GridTileType::buildable)
    {
        enable();
    }
    else
    {
        disable();
    }

    if (bn::keypad::a_pressed() && targeting_buildable_grid)
    {
        level->add_tower(camera_position);
    }
}
