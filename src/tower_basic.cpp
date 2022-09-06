#include "tower_basic.h"

using namespace cd;

TowerBasic::TowerBasic(bn::camera_ptr _camera, bn::fixed_point _position) : Tower(_camera, _position)
{
    sprite = bn::sprite_items::tower_basic.create_sprite(0, 0);

    sprite.value().set_camera(_camera);
    sprite.value().set_visible(true);
    sprite.value().set_z_order(-2);

    aggro_range = 32;
    damage = 10;
    bullet_speed = 0.02;
    cost = 10;
    fire_rate_per_sec = 1;
}

TowerBasic::~TowerBasic()
{
}

bn::fixed_rect TowerBasic::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite.value().dimensions().width() / 2,
        sprite.value().dimensions().height() / 2);
}

void TowerBasic::set_animation_shoot_right()
{
    set_animation_shoot_left();
    sprite.value().set_horizontal_flip(true);
}

void TowerBasic::set_animation_shoot_left()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 0)
    {
        return;
    }
    sprite.value().set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void TowerBasic::set_animation_shoot_up()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 8)
    {
        return;
    }
    sprite.value().set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        8, 9, 10, 11, 12, 13, 14, 15);
}

void TowerBasic::set_animation_shoot_down()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 16)
    {
        return;
    }
    sprite.value().set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        16, 17, 18, 19, 20, 21, 22, 23);
}

void TowerBasic::set_position(bn::fixed x, bn::fixed y)
{
    Tower::set_position(x, y);

    bullet_start_position.set_y(bullet_start_position.y() - 8);

    if (sprite.has_value())
    {
        sprite.value().set_position(bn::fixed_point(position.x(), position.y() - 8));
    }
}
