#include "cursor.h"

using namespace cd;

Cursor::Cursor(bn::camera_ptr _camera) : camera(_camera)
{
    bn::blending::set_transparency_alpha(0.4); // for the tower preview

    sprite = bn::sprite_items::cursor.create_sprite(0, 0);

    position = bn::fixed_point(0, 0);
    sprite.value().set_camera(_camera);
    sprite.value()
        .set_position(position);
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

bool Cursor::can_build(Level *level)
{
    GridTileType top_left_grid = level->get_map_cell(
        position.x() - 8,
        position.y() - 8);
    GridTileType bottom_right_grid = level->get_map_cell(
        position.x() + 7,
        position.y() + 7);
    // TODO check other corners

    for (Tower &tower : *level->get_towers())
    {
        if (tower.get_hitbox().intersects(get_hitbox()))
        {
            return false;
        }
    }

    if (top_left_grid == bottom_right_grid && top_left_grid == GridTileType::buildable)
    {
        return true;
    }

    return false;
}

void Cursor::on_tick(Level *level)
{
    if (can_build(level))
    {
        enable();
    }
    else
    {
        disable();
    }

    if (bn::keypad::a_pressed() && targeting_buildable_grid && current_selection.has_value())
    {
        level->add_tower(position, current_selection.value());
        remove_current_selection();
    }

    if (tower_select_open)
    {
        if (bn::keypad::right_pressed())
        {
            set_selection(TowerType::AoE);
        }

        if (bn::keypad::left_pressed())
        {
            set_selection(TowerType::Sticky);
        }

        if (bn::keypad::up_pressed())
        {
            set_selection(TowerType::Basic);
        }

        if (bn::keypad::down_pressed() || bn::keypad::b_pressed())
        {
            hide_tower_select_ui();
        }
    }
    else
    {
        if (bn::keypad::b_pressed())
        {
            open_tower_select_ui();
        }

        if (bn::keypad::right_pressed())
        {
            position.set_x(position.x() + 8);
        }

        if (bn::keypad::left_pressed())
        {
            position.set_x(position.x() - 8);
        }

        if (bn::keypad::down_pressed())
        {
            position.set_y(position.y() + 8);
        }

        if (bn::keypad::up_pressed())
        {
            position.set_y(position.y() - 8);
        }
    }

    if (placeholder.has_value())
    {
        placeholder.value().set_position(position);
    }

    if (range.has_value())
    {
        range.value().set_position(position);
    }

    if (tower_select_ui.has_value())
    {
        tower_select_ui.value().set_camera(camera);
        tower_select_ui.value().set_position(position);
    }

    update_camera(level->get_bg().value());
    sprite.value().set_position(position);
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
        sprite.value().dimensions().width(),
        sprite.value().dimensions().height());
}

void Cursor::remove_current_selection()
{
    current_selection.reset();
    range.reset();
    placeholder.reset();
}

void Cursor::hide_tower_select_ui()
{
    tower_select_ui.reset();
    overlay_bg.reset();
    tower_select_open = false;
}

void Cursor::open_tower_select_ui()
{
    tower_select_open = true;
    remove_current_selection();

    tower_select_ui = bn::regular_bg_items::tower_select_ui.create_bg(0, 0);
    tower_select_ui.value().set_priority(0);

    overlay_bg = bn::regular_bg_items::overlay_bg.create_bg(0, 0);
    overlay_bg.value().set_camera(camera);
    overlay_bg.value().set_visible(false);
    overlay_bg.value().set_priority(1);
    overlay_bg.value().set_blending_enabled(true);

    overlay_bg.value()
        .set_visible(true);
}

void Cursor::set_selection(TowerType type)
{
    hide_tower_select_ui();
    current_selection = type;

    range = bn::affine_bg_items::range.create_bg(0, 0);
    range.value().set_camera(camera);
    range.value().set_visible(true);
    range.value().set_wrapping_enabled(false);
    range.value().set_priority(0);
    range.value().set_horizontal_scale(1);
    range.value().set_vertical_scale(1);

    placeholder = Tower::get_sprite(current_selection.value()).create_sprite(0, 0);
    placeholder.value().set_camera(camera);
    placeholder.value()
        .set_visible(true);
    placeholder.value().set_blending_enabled(true);
}
