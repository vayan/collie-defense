#include "shop.h"
#include "game.h"
#include "player.h"

using namespace cd;

Shop::Shop(bn::camera_ptr _camera) : camera(_camera)
{

    select_highlight = bn::sprite_items::store_select.create_sprite(78, -30);
    select_highlight->set_bg_priority(0);

    store = bn::regular_bg_items::store.create_bg(0, 0);
    store->set_priority(0);

    overlay_bg = bn::regular_bg_items::overlay_bg.create_bg(0, 0);
    overlay_bg->set_camera(camera);
    overlay_bg->set_visible(false);
    overlay_bg->set_priority(1);
    overlay_bg->set_blending_enabled(true);

    overlay_bg.value()
        .set_visible(true);

    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::Basic, -30));
    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::AoE, 0));
    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::Sticky, 30));

    bn::sprite_text_generator _sprite_text_generator(as::fixed_font_8x8);
    text_generator = _sprite_text_generator;
}

Shop::~Shop()
{
}

void Shop::on_tick(Game *game)
{
    if (bn::keypad::b_pressed())
    {
        open = false;
    }

    if (bn::keypad::a_pressed())
    {
        purchase = elements.at(current_element).first;
        open = false;
    }

    if (bn::keypad::down_pressed())
    {
        bn::sound_items::select.play();
        select_next();
    }

    if (bn::keypad::up_pressed())
    {
        bn::sound_items::select.play();
        select_previous();
    }

    select_highlight.value()
        .set_y(elements.at(current_element).second);

    refresh_money(game->get_player());
    display_level_count(game->get_current_level_index() + 1, cd::number_of_levels);
}

bn::optional<TowerType> Shop::get_purchase()
{
    return purchase;
}

bool Shop::is_closed()
{
    return !open;
}

void Shop::select_next()
{
    current_element += 1;
    if (current_element == elements.size())
    {
        current_element = 0;
    }
}

void Shop::select_previous()
{
    current_element -= 1;

    if (current_element < 0)
    {
        current_element = elements.size() - 1;
    }
}

void Shop::display_level_count(bn::fixed current_level_index, bn::fixed total_level_count)
{
    bn::string<50> text = bn::format<50>("STAGE {}/{}", current_level_index, total_level_count);
    text_sprites_level.clear();
    text_generator.value()
        .generate(bn::fixed_point(-115, 56), text, text_sprites_level);

    for (bn::sprite_ptr text_sprite : text_sprites_level)
    {
        text_sprite.set_bg_priority(0);
    }
}

void Shop::refresh_money(Player *player)
{
    bn::string<50> text = bn::format<50>("{}$", player->get_money());
    text_sprites.clear();
    text_generator.value()
        .generate(bn::fixed_point(55, -56), text, text_sprites);

    for (bn::sprite_ptr text_sprite : text_sprites)
    {
        text_sprite.set_bg_priority(0);
    }
}
