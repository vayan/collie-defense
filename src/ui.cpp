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
    bn::string<50> text = bn::format<50>("life: {} | money: {}", player->get_life(), player->get_money());
    text_sprites.clear();
    text_generator.value()
        .generate(bn::fixed_point(-110, -70), text, text_sprites);
}
