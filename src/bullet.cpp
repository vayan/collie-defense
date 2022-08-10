#include "bullet.h"

using namespace cd;

Bullet::Bullet(
    bn::camera_ptr _camera,
    bn::fixed_point _position,
    Target *_target) : camera(_camera),
                       position(_position),
                       target(_target)
{
    sprite = bn::sprite_items::bullet.create_sprite(0, 0);

    sprite.value()
        .set_position(position);
    sprite.value().set_camera(camera);
    sprite.value().set_visible(true);
}

Bullet::~Bullet()
{
}

void Bullet::on_tick(Level *level)
{
    progress = progress + delta;

    position = lerp_points(position, target->get_position(), progress);

    if (progress >= 1)
    {
        destroyed = true;
    }

    sprite.value().set_position(position);
}

bool Bullet::to_be_destroyed()
{
    return destroyed;
}
