#include "shop.h"
#include "player.h"

using namespace cd;

Shop::Shop(bn::camera_ptr _camera) : camera(_camera)
{

    select_highlight = bn::sprite_items::tower_select_ui_highlight.create_sprite(83, -30);
    select_highlight.value().set_bg_priority(0);

    tower_select_ui = bn::regular_bg_items::tower_select_ui.create_bg(0, 0);
    tower_select_ui.value().set_priority(2);

    overlay_bg = bn::regular_bg_items::overlay_bg.create_bg(0, 0);
    overlay_bg.value().set_camera(camera);
    overlay_bg.value().set_visible(false);
    overlay_bg.value().set_priority(3);
    overlay_bg.value().set_blending_enabled(true);

    overlay_bg.value()
        .set_visible(true);

    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::Basic, -30));
    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::Sticky, 0));
    elements.push_back(bn::pair<TowerType, bn::fixed>(TowerType::AoE, 30));

    bn::sprite_text_generator _sprite_text_generator(as::fixed_font_8x8);
    text_generator = _sprite_text_generator;
}

Shop::~Shop()
{
}

void Shop::on_tick(Level *level, Player *player)
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
        select_next();
    }

    if (bn::keypad::up_pressed())
    {
        select_previous();
    }

    select_highlight.value()
        .set_y(elements.at(current_element).second);

    refresh_money(player);
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

void Shop::refresh_money(Player *player)
{
    bn::string<50> text = bn::format<50>("{}$$", player->get_money());
    text_sprites.clear();
    text_generator.value()
        .generate(bn::fixed_point(68, -56), text, text_sprites);

    for (bn::sprite_ptr text_sprite : text_sprites)
    {
        text_sprite.set_bg_priority(0);
    }
}
