#include "bullet.h"
#include "player.h"

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

void Bullet::on_tick(Level *level, Player *player)
{
    progress = progress + delta;

    position = lerp_points(position, target->get_position(), progress);

    sprite.value().set_position(position);

    if (get_hitbox().intersects(target->get_hitbox()))
    {
        player->on_target_killed(target);
        destroyed = true;
        target->hit(damage);
    }
}

bool Bullet::to_be_destroyed()
{
    return destroyed;
}

bn::fixed_rect Bullet::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite.value().dimensions().width(),
        sprite.value().dimensions().height());
}
