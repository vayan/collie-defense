#include "ui.h"

using namespace cd;

UI::UI()
{
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

    bn::string<50> text = bn::format<50>("life: {}", player->get_life());
    text_sprites.clear();
    text_generator.value()
        .generate(bn::fixed_point(-114, -72), text, text_sprites);
}

bool UI::is_paused()
{
    return paused;
}
