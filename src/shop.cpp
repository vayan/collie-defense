#include "shop.h"
#include "game.h"
#include "player.h"

using namespace cd;

Shop::Shop(bn::camera_ptr _camera, bn::vector<TowerType, 4> available_towers) : camera(_camera)
{

    select_highlight = bn::regular_bg_items::store_select.create_bg(78, 0);
    select_highlight->set_priority(0);

    store = bn::regular_bg_items::store.create_bg(0, 0);
    store->set_priority(1);

    overlay_bg = bn::regular_bg_items::overlay_bg.create_bg(0, 0);
    overlay_bg->set_camera(camera);
    overlay_bg->set_visible(false);
    overlay_bg->set_priority(2);
    overlay_bg->set_blending_enabled(true);

    overlay_bg.value()
        .set_visible(true);

    for (TowerType type : available_towers)
    {
        switch (type)
        {
        case TowerType::Magic:
            elements.push_back(TowerMagic(camera, bn::fixed_point(65, 25)));
            break;
        case TowerType::Arrow:
            elements.push_back(TowerBallista(camera, bn::fixed_point(90, -25)));
            break;
        case TowerType::Canon:
            elements.push_back(TowerBasic(camera, bn::fixed_point(65, -25)));
            break;
        case TowerType::Catapult:
            elements.push_back(TowerCatapult(camera, bn::fixed_point(90, 25)));
            break;
        default:
            log("this type of tower cannot be added to store");
            break;
        }
    }

    for (Tower element : elements)
    {
        element.get_sprite()->set_bg_priority(0);
        element.set_position(element.get_position());
    }

    bn::sprite_text_generator _sprite_text_generator(cd::fixed_font_8x8);
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
        purchase = elements.at(current_element).get_type();
        open = false;
    }

    if (bn::keypad::right_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        current_element += 1;
    }

    if (bn::keypad::down_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        current_element += 2;
    }

    if (bn::keypad::left_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        current_element -= 1;
    }

    if (bn::keypad::up_pressed())
    {
        play_sfx(bn::sound_items::select, 1);
        current_element -= 2;
    }

    if (current_element < 0)
    {
        current_element = elements.size() - 1;
    }

    if (current_element >= elements.size())
    {
        current_element = 0;
    }

    select_highlight.value().set_position(elements.at(current_element).get_position().x() + 12, elements.at(current_element).get_position().y() + 10);

    refresh_money(game->get_player());
    text_sprites_level.clear();
    display_level_count(game->get_current_level_index() + 1);
    display_tower_info();
}

bn::optional<TowerType> Shop::get_purchase()
{
    return purchase;
}

bool Shop::is_closed()
{
    return !open;
}

void Shop::display_level_count(bn::fixed current_level_index)
{
    bn::string<50> text = bn::format<50>("LEVEL {}", current_level_index);
    text_generator.value()
        .generate(bn::fixed_point(-111, 70), text, text_sprites_level);

    for (bn::sprite_ptr text_sprite : text_sprites_level)
    {
        text_sprite.set_bg_priority(0);
    }
}

bn::string<6> Shop::get_tower_normalize_to_star(bn::fixed value, bn::fixed min, bn::fixed max)
{
    bn::string<6> text = "*";
    bn::fixed number_normalized = 1.0 + ((value - min).safe_division(max - min)).safe_multiplication(4.0).floor_integer();

    for (int i = 1; i < number_normalized; i++)
    {
        text += "*";
    }

    return text;
}

void Shop::display_tower_info()
{
    Tower *tower = &elements.at(current_element);
    bn::string<50> text = bn::format<50>("{}$", tower->get_cost());

    // TODO update those min / max values to get accurate star equivalent
    bn::string<50> info_range = bn::format<50>("RANGE:{}", get_tower_normalize_to_star(tower->get_aggro_range(), 18, 60));
    bn::string<50> info_power = bn::format<50>("POWER:{}", get_tower_normalize_to_star(tower->get_dps(), 9, 40));
    bn::string<50> info_speed = bn::format<50>("SPEED:{}", get_tower_normalize_to_star(tower->get_speed().safe_multiplication(10).floor_integer(), 8, 10));

    text_sprites_tower.clear();
    text_generator.value()
        .generate(bn::fixed_point(tower->get_position().x() - 11, tower->get_position().y() + 27), text, text_sprites_tower);
    text_generator.value()
        .generate(bn::fixed_point(-45, tower->get_position().y() - 10), info_range, text_sprites_tower);
    text_generator.value()
        .generate(bn::fixed_point(-45, tower->get_position().y()), info_power, text_sprites_tower);
    text_generator.value()
        .generate(bn::fixed_point(-45, tower->get_position().y() + 10), info_speed, text_sprites_tower);

    for (bn::sprite_ptr text_sprite : text_sprites_tower)
    {
        text_sprite.set_bg_priority(0);
        text_sprite.set_z_order(0);
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
