#include "shop.h"

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
}

Shop::~Shop()
{
}

void Shop::on_tick(Level *level)
{
    if (bn::keypad::b_pressed())
    {
        open = false;
    }

    if (bn::keypad::a_pressed())
    {
        purchase = TowerType::Basic;
        open = false;
    }

    if (bn::keypad::down_pressed())
    {
        select_highlight.value().set_y(select_highlight.value().y() + 30);
    }

    if (bn::keypad::up_pressed())
    {
        select_highlight.value().set_y(select_highlight.value().y() - 30);
    }
}

bn::optional<TowerType> Shop::get_purchase()
{
    return purchase;
}

bool Shop::is_closed()
{
    return !open;
}
