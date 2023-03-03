#include "tower_basic.h"

using namespace cd;

TowerBasic::TowerBasic(bn::camera_ptr _camera, bn::fixed_point _position) : Tower(_camera, _position)
{
    sprite = bn::sprite_items::tower_basic.create_sprite(0, 0);

    sprite->set_camera(_camera);
    sprite->set_visible(true);
    sprite->set_z_order(-2);

    aggro_range = 32;
    damage = 9;
    bullet_speed = 1;
    cost = 10;
    fire_rate_per_sec = 1;

    type = TowerType::Canon;

    bullet_sprite = bn::sprite_items::bullet_canon;
}

TowerBasic::~TowerBasic()
{
}

bn::fixed_rect TowerBasic::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite->dimensions().width() / 2,
        sprite->dimensions().height() / 2);
}

void TowerBasic::set_animation_shoot_right()
{
    set_animation_shoot_left();
    sprite->set_horizontal_flip(true);
    bullet_start_position_offset = bn::fixed_point(9, -16);
}

void TowerBasic::set_animation_shoot_left()
{
    bullet_start_position_offset = bn::fixed_point(-8, -16);
    sprite->set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void TowerBasic::set_animation_shoot_up()
{
    bullet_start_position_offset = bn::fixed_point(6, -16);
    sprite->set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        8, 9, 10, 11, 12, 13, 14, 15);
}

void TowerBasic::set_animation_shoot_down()
{
    bullet_start_position_offset = bn::fixed_point(0, -6);
    sprite->set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_basic.tiles_item(),
        16, 17, 18, 19, 20, 21, 22, 23);
}

void TowerBasic::set_position(bn::fixed x, bn::fixed y)
{
    Tower::set_position(x, y);

    if (sprite.has_value())
    {
        sprite->set_position(bn::fixed_point(position.x(), position.y() - 8));
    }
}
