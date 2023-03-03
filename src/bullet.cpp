#include "bullet.h"
#include "game.h"
#include "player.h"

using namespace cd;

Bullet::Bullet(
    bn::camera_ptr _camera,
    bn::fixed_point _position,
    Target *_target,
    bn::fixed _speed_delta,
    bn::fixed _damage,
    bn::sprite_item _sprite) : camera(_camera),
                               position(_position),
                               target(_target),
                               delta(_speed_delta),
                               damage(_damage)

{
    sprite = _sprite.create_sprite(position);

    sprite->set_camera(camera);
    sprite->set_visible(true);
    sprite->set_z_order(-10);
    target_id = target->get_id();
}

Bullet::~Bullet()
{
}

void Bullet::on_tick(Game *game)
{
    if (!target->is_active() || target->get_id() != target_id)
    {
        destroyed = true;
        return;
    }

    position = move_to(position, target->get_position(), delta);

    sprite->set_position(position);

    if (get_hitbox().intersects(target->get_hitbox()) && target->get_id() == target_id)
    {
        destroyed = true;
        log("bullet hit target", target_id);

        target->hit(damage, game->get_player());
    }
}

bool Bullet::to_be_destroyed()
{
    return destroyed || !target->is_active();
}

bn::fixed_rect Bullet::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite->dimensions().width() / 2,
        sprite->dimensions().height() / 2);
}
