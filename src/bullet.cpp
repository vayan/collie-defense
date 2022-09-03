#include "bullet.h"
#include "player.h"

using namespace cd;

Bullet::Bullet(
    bn::camera_ptr _camera,
    bn::fixed_point _position,
    Target *_target,
    bn::fixed _speed_delta,
    bn::fixed _damage) : camera(_camera),
                         position(_position),
                         target(_target),
                         delta(_speed_delta),
                         damage(_damage)

{
    sprite = bn::sprite_items::bullet.create_sprite(0, 0);

    sprite.value()
        .set_position(position);
    sprite.value().set_camera(camera);
    sprite.value().set_visible(true);
    target_id = target->get_id();
}

Bullet::~Bullet()
{
}

void Bullet::on_tick(Level *level, Player *player)
{
    if (target->get_id() != target_id)
    {
        destroyed = true;
        return;
    }

    progress = progress + delta;

    position = lerp_points(position, target->get_position(), progress);

    sprite.value().set_position(position);

    if (get_hitbox().intersects(target->get_hitbox()) && target->get_id() == target_id)
    {
        destroyed = true;
        target->hit(damage, player);
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
