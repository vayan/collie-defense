#include "tower_magic.h"

using namespace cd;

TowerMagic::TowerMagic(bn::camera_ptr _camera, bn::fixed_point _position) : Tower(_camera, _position)
{
    sprite = bn::sprite_items::tower_magic.create_sprite(0, 0);

    sprite->set_camera(_camera);
    sprite->set_visible(true);
    sprite->set_z_order(-2);

    aggro_range = 18;
    damage = 15;
    bullet_speed = 1.1;
    cost = 30;
    fire_rate_per_sec = 1;
    dps = fire_rate_per_sec.safe_multiplication(damage);

    bullet_sprite = bn::sprite_items::bullet_magic;

    type = TowerType::Magic;

    bullet_start_position_offset = bn::fixed_point(0, -18);
}

TowerMagic::~TowerMagic()
{
}

bn::fixed_rect TowerMagic::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite->dimensions().width() / 2,
        sprite->dimensions().height());
}

void TowerMagic::set_animation_shoot_right()
{
    set_animation_shoot_left();
}

void TowerMagic::set_animation_shoot_left()
{
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_magic.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void TowerMagic::set_animation_shoot_up()
{
    set_animation_shoot_left();
}

void TowerMagic::set_animation_shoot_down()
{
    set_animation_shoot_left();
}

void TowerMagic::set_position(bn::fixed x, bn::fixed y)
{
    Tower::set_position(x, y);

    if (sprite.has_value())
    {
        sprite->set_position(bn::fixed_point(position.x(), position.y() - 8));
    }
}
