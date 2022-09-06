#include "tower_ballista.h"

using namespace cd;

TowerBallista::TowerBallista(bn::camera_ptr _camera, bn::fixed_point _position) : Tower(_camera, _position)
{
    sprite = bn::sprite_items::tower_ballista.create_sprite(0, 0);

    sprite.value().set_camera(_camera);
    sprite.value().set_visible(true);
    sprite.value().set_z_order(-2);

    aggro_range = 24;
    damage = 20;
    bullet_speed = 0.01;
    cost = 50;
    fire_rate_per_sec = 1;
}

TowerBallista::~TowerBallista()
{
}

void TowerBallista::set_animation_shoot_right()
{
    set_animation_shoot_left();
}

void TowerBallista::set_animation_shoot_left()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 0)
    {
        return;
    }
    animation = bn::create_sprite_animate_action_once(
        sprite.value(),
        8,
        bn::sprite_items::tower_ballista.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void TowerBallista::set_animation_shoot_up()
{
    set_animation_shoot_left();
}

void TowerBallista::set_animation_shoot_down()
{
    set_animation_shoot_left();
}
