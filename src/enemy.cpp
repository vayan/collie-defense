#include "enemy.h"
#include "player.h"

using namespace cd;

Enemy::Enemy(
    bn::camera_ptr _camera,
    bn::fixed_point _origin,
    bn::fixed_point **_steps,
    bn::fixed _steps_number) : camera(_camera),
                               origin(_origin),
                               steps(_steps),
                               steps_number(_steps_number)
{
    sprite = bn::sprite_items::sheep.create_sprite(0, 0);

    position = origin;
    from = origin;
    to = *steps[0];

    sprite.value()
        .set_position(from);
    sprite.value().set_camera(camera);
    sprite.value().set_visible(true);
}

Enemy::~Enemy()
{
}

void Enemy::on_tick(Level *level, Player *player)
{
    if (current_step == steps_number)
    {
        // remove
        player->on_hit(this);
        dead = true;
        return;
    }
    progress = progress + delta;

    position = lerp_points(from, to, progress);

    if (progress >= 1)
    {
        current_step += 1;
        from = to;
        to = *steps[current_step.integer()];
        progress = 0;
        // TODO check no more steps
    }

    sprite.value().set_position(position);
}

bool Enemy::is_dead()
{
    return dead;
}

bn::fixed_point Enemy::get_position()
{
    return position;
}

void Enemy::hit()
{
    dead = true;
}

bn::fixed_rect Enemy::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite.value().dimensions().width(),
        sprite.value().dimensions().height());
}

bn::fixed Enemy::get_reward()
{
    return 10; // TODO get dynamic value
}

bn::fixed Enemy::get_strenght()
{
    return 10; // TODO get dynamic value
}
