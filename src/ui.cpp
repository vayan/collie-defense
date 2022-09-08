#include "ui.h"

using namespace cd;

UI::UI()
{
    bn::fixed half_display_width = bn::display::width() / 2;
    bn::fixed half_display_height = bn::display::height() / 2;

    player_life_sprite = bn::sprite_items::player_life.create_sprite(-half_display_width + 40, -half_display_height + 12);

    player_life_sprite.value().set_visible(true);
    player_life_sprite.value().set_z_order(-2);

    bn::sprite_text_generator _sprite_text_generator(as::fixed_font_8x8);
    text_generator = _sprite_text_generator;
}

UI::~UI()
{
}

void UI::on_tick(Level *level, Player *player)
{
    if (bn::keypad::start_pressed())
    {
        paused = !paused;
    }

    int life_sprite_index = player->get_life().safe_division(10).ceil_integer() - 1;
    if (life_sprite_index < 0)
    {
        life_sprite_index = 0;
    }

    player_life_sprite.value()
        .set_item(bn::sprite_items::player_life, life_sprite_index);
}

bool UI::is_paused()
{
    return paused;
}
