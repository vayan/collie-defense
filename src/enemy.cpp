#include "enemy.h"

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

void Enemy::on_tick()
{
    if (current_step == steps_number)
    {
        // remove
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
