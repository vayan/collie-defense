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
    // no idea if set_item is fast... let's see later
    sprite.value().set_item(bn::sprite_items::cursor, 0);
}

void Cursor::disable()
{
    sprite.value().set_item(bn::sprite_items::cursor, 1);
}

void Cursor::on_tick(Level *level)
{
    disable();
}
