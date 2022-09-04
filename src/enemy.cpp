#include "enemy.h"
#include "player.h"

using namespace cd;

Enemy::Enemy(
    bn::fixed _id,
    bn::camera_ptr _camera,
    bn::fixed_point _origin,
    bn::fixed_point **_steps,
    bn::fixed _steps_number) : id(_id),
                               camera(_camera),
                               origin(_origin),
                               steps(_steps),
                               steps_number(_steps_number)
{
    sprite = bn::sprite_items::sheep.create_sprite(0, 0);
    life_bar = bn::sprite_items::life_bar.create_sprite(0, 0);

    position = origin;
    from = origin;
    to = *steps[0];

    life_bar.value().set_camera(camera);
    life_bar.value().set_visible(false);

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

    progress += speed / (distance(from, to) * 100);

    position = lerp_points(from, to, progress);

    if (progress >= 1)
    {
        current_step += 1;
        from = to;
        to = *steps[current_step.integer()];
        progress = 0;
        animation.reset();
    }

    if (!animation.has_value())
    {
        update_animation();
    }

    if (life < max_life)
    {
        life_bar.value().set_visible(true);
    }

    life_bar.value().set_position(bn::fixed_point(position.x(), position.y() - 8));
    sprite.value().set_position(position);

    if (animation.has_value())
    {
        animation.value().update();
    }
}

bool Enemy::is_dead()
{
    return dead;
}

bn::fixed_point Enemy::get_position()
{
    return position;
}

void Enemy::hit(bn::fixed dmg, Player *player)
{
    life -= dmg;

    if (life <= 0)
    {
        player->on_target_killed(this);
        dead = true;
    }

    bn::fixed progress_index = life.safe_multiplication(11).safe_division(100).round_integer();
    if (progress_index < 0)
    {
        // TODO check why this happen
        progress_index = 0;
    }
    life_bar.value()
        .set_item(
            bn::sprite_items::life_bar,
            progress_index.integer());
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

bn::fixed Enemy::get_id()
{
    return id;
}

void Enemy::set_animation_right_walk()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 0)
    {
        return;
    }
    sprite.value().set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void Enemy::set_animation_left_walk()
{
    set_animation_right_walk();
    sprite.value()
        .set_horizontal_flip(true);
}

void Enemy::set_animation_down_walk()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 16)
    {
        return;
    }
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        16, 17, 18, 19, 20, 21, 22, 23);
}

void Enemy::set_animation_up_walk()
{
    if (animation.has_value() && animation.value().graphics_indexes().front() == 32)
    {
        return;
    }
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        32, 33, 34, 35, 36, 37, 38, 39);
}

void Enemy::update_animation()
{
    bn::fixed deg = bn::degrees_atan2(
        from.y().integer() - to.y().integer(),
        from.x().integer() - to.x().integer());

    if (deg > 45 && deg < 135)
    {
        set_animation_up_walk();
    }
    else if (deg < -45 && deg > -135)
    {
        set_animation_down_walk();
    }
    else if (deg <= 45 && deg >= -45)
    {
        set_animation_left_walk();
    }
    else
    {
        set_animation_right_walk();
    }
}
