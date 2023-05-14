#include "cursor.h"
#include "game.h"
#include "player.h"

using namespace cd;

Cursor::Cursor(bn::camera_ptr _camera) : camera(_camera)
{
    bn::blending::set_transparency_alpha(0.4); // for the tower preview

    sprite = bn::sprite_items::cursor.create_sprite(0, 0);

    position = bn::fixed_point(0, 0);
    sprite->set_camera(_camera);
    sprite.value()
        .set_position(position);
    hide();
}

Cursor::~Cursor()
{
}

void Cursor::enable()
{
    targeting_buildable_grid = true;
    sprite->set_item(bn::sprite_items::cursor, 0);
}

void Cursor::disable()
{
    targeting_buildable_grid = false;
    sprite.value()
        .set_item(bn::sprite_items::cursor, 1);
}

bool Cursor::can_build(Level *level)
{
    for (Tower *tower : *level->get_towers())
    {
        if (tower->get_hitbox().intersects(get_hitbox()))
        {
            return false;
        }
    }

    GridTileType top_left_grid = level->get_map_cell(
        position.x() - 8,
        position.y() - 8);
    GridTileType top_right_grid = level->get_map_cell(
        position.x() + 7,
        position.y() - 8);
    GridTileType bottom_right_grid = level->get_map_cell(
        position.x() + 7,
        position.y() + 7);
    GridTileType bottom_left_grid = level->get_map_cell(
        position.x() - 7,
        position.y() + 7);

    if (
        top_left_grid == bottom_right_grid &&
        bottom_right_grid == bottom_left_grid &&
        top_right_grid == bottom_left_grid &&
        top_left_grid == GridTileType::buildable)
    {
        return true;
    }

    return false;
}

bool Cursor::long_key_press_logic(bn::keypad::key_type key)
{
    if (bn::keypad::pressed(key) || bn::keypad::held(key))
    {
        bn::fixed time_since_last_press = 0;

        if (bn::keypad::held(key) && frame_elapse_since_last_press.first == key)
        {
            time_since_last_press = frame_elapse_since_last_press.second + 1;
        }

        frame_elapse_since_last_press = bn::pair(key, time_since_last_press);
    }

    if (bn::keypad::released(key))
    {
        frame_elapse_since_last_press = bn::pair(key, 0);
    }

    // when a long press happens, only start moving after a few frames (makes it easier to do a single press)
    // and every other frame (to slow it down).
    return frame_elapse_since_last_press.first == key &&
           (frame_elapse_since_last_press.second > 20 && frame_elapse_since_last_press.second.integer() % 2 == 0);
}

void Cursor::on_tick(Game *game)
{
    if (can_build(game->get_current_level()))
    {
        enable();
    }
    else
    {
        disable();
    }

    if (
        bn::keypad::a_pressed() &&
        targeting_buildable_grid &&
        current_selection.has_value())
    {
        bn::fixed cost = current_selection.value()->get_cost();

        if (game->get_player()->get_money() >= cost)
        {
            bool success = game->get_current_level()->add_tower(current_selection.value());

            if (success)
            {
                game->get_player()->spend_money(cost);
            }
            remove_current_selection(!success);
        }
        else
        {
            // TODO show no money warning
        }
    }

    if (shop.has_value())
    {
        shop->on_tick(game);

        if (shop->get_purchase().has_value())
        {
            set_selection(shop->get_purchase().value());
        }
        if (shop.has_value() && shop->is_closed())
        {
            hide_shop();
        }
    }
    else
    {
        if (bn::keypad::b_pressed())
        {
            show_shop(game);
        }

        if (bn::keypad::right_pressed() || long_key_press_logic(bn::keypad::key_type::RIGHT))
        {
            position.set_x(position.x() + 8);
        }

        if (bn::keypad::left_pressed() || long_key_press_logic(bn::keypad::key_type::LEFT))
        {
            position.set_x(position.x() - 8);
        }

        if (bn::keypad::down_pressed() || long_key_press_logic(bn::keypad::key_type::DOWN))
        {
            position.set_y(position.y() + 8);
        }

        if (bn::keypad::up_pressed() || long_key_press_logic(bn::keypad::key_type::UP))
        {
            position.set_y(position.y() - 8);
        }
    }

    if (current_selection.has_value())
    {
        current_selection.value()->set_position(position.x(), position.y());
    }

    if (range.has_value())
    {
        range->set_position(position);
    }

    check_screen_bounds();

    // can re-enable this if I do levels bigger than 240×160
    // update_camera(level->get_bg().value());
    sprite.value()
        .set_position(position);
}

void Cursor::update_camera(bn::regular_bg_ptr map)
{
    bn::fixed x = position.x();
    bn::fixed y = position.y();
    bn::fixed half_map_width = map.dimensions().width() / 2;
    bn::fixed half_map_height = map.dimensions().height() / 2;
    bn::fixed half_display_width = bn::display::width() / 2;
    bn::fixed half_display_height = bn::display::height() / 2;

    bn::fixed min_x = -(half_map_width - half_display_width);
    bn::fixed max_x = half_map_width - half_display_width;
    bn::fixed min_y = -(half_map_height - half_display_height);
    bn::fixed max_y = half_map_height - half_display_height;

    if (x < min_x)
    {
        x = min_x;
    }
    else if (x > max_x)
    {
        x = max_x;
    }
    else
    {
        x = x;
    }

    if (y < min_y)
    {
        y = min_y;
    }
    else if (y > max_y)
    {
        y = max_y;
    }
    else
    {
        y = y;
    }

    camera.set_position(x, y);
}

bn::fixed_rect Cursor::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite->dimensions().width(),
        sprite->dimensions().height());
}

void Cursor::remove_current_selection(bool hard_clean)
{
    if (current_selection.has_value() && hard_clean)
    {
        delete current_selection.value();
    }
    current_selection.reset();
    range.reset();
}

void Cursor::hide_shop()
{
    if (shop.has_value())
    {
        shop.reset();
    }
}

void Cursor::show_shop(Game *game)
{
    remove_current_selection(true);
    shop = Shop(camera, game->get_current_level()->get_available_towers());
}

void Cursor::set_selection(TowerType type)
{
    hide_shop();

    switch (type)
    {
    case TowerType::Magic:
        current_selection = new TowerMagic(camera, position);
        break;
    case TowerType::Arrow:
        current_selection = new TowerBallista(camera, position);
        break;
    case TowerType::Canon:
        current_selection = new TowerBasic(camera, position);
        break;
    case TowerType::Catapult:
        current_selection = new TowerCatapult(camera, position);
        break;
    default:
        log("this type of tower cannot be created for now");
        break;
    }

    // range background is 128x128px
    bn::fixed scale = current_selection.value()->get_aggro_range().safe_division(64);

    range = bn::affine_bg_items::range.create_bg(0, 0);
    range->set_camera(camera);
    range->set_visible(true);
    range->set_wrapping_enabled(false);
    range->set_blending_enabled(true);
    range->set_priority(0);
    range->set_horizontal_scale(scale);
    range->set_vertical_scale(scale);
}

void Cursor::check_screen_bounds()
{
    bn::fixed x = position.x();
    bn::fixed y = position.y();
    bn::fixed half_display_width = bn::display::width() / 2;
    bn::fixed half_display_height = bn::display::height() / 2;

    if (x < -half_display_width)
    {
        x = -half_display_width;
    }
    else if (x > half_display_width)
    {
        x = half_display_width;
    }

    if (y < -half_display_height)
    {
        y = -half_display_height;
    }
    else if (y > half_display_height)
    {
        y = half_display_height;
    }

    position.set_x(x);
    position.set_y(y);
}

void Cursor::hide()
{
    sprite->set_visible(false);
}

void Cursor::show()
{
    sprite->set_visible(true);
}
