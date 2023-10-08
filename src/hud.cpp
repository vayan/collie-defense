#include "hud.h"
#include "game.h"

using namespace cd;

HUD::HUD()
{
    bn::fixed half_display_width = bn::display::width() / 2;
    bn::fixed half_display_height = bn::display::height() / 2;

    player_life_sprite = bn::sprite_items::player_life.create_sprite(-half_display_width + 40, -half_display_height + 12);

    player_life_sprite->set_visible(true);
    player_life_sprite->set_z_order(-100);

    bn::sprite_text_generator _sprite_text_generator(cd::fixed_font_8x8);
    text_generator = _sprite_text_generator;
}

HUD::~HUD()
{
    log("destroying hud");
    player_life_sprite.reset();
    text_generator.reset();
    text_sprites.clear();
}

void HUD::set_state(bool activate)
{

    if (activate == false)
    {
        player_life_sprite->set_visible(false);
        return;
    }

    player_life_sprite->set_visible(true);
}

void HUD::on_tick(Game *game)
{
    int life_sprite_index = game->get_player()->get_life().safe_division(10).ceil_integer() - 1;
    if (life_sprite_index < 0)
    {
        life_sprite_index = 0;
    }

    player_life_sprite.value()
        .set_item(bn::sprite_items::player_life, life_sprite_index);
}
