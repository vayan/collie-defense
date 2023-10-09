#include "enemy.h"
#include "game.h"
#include "player.h"

using namespace cd;

Enemy::Enemy(
    const bn::fixed _id,
    bn::camera_ptr _camera,
    const bn::fixed_point _origin,
    const bn::fixed_point **_steps,
    const bn::fixed _steps_number) : id(_id),
                                     camera(_camera),
                                     origin(_origin),
                                     steps(_steps),
                                     steps_number(_steps_number)
{
    life_bar = bn::sprite_items::life_bar.create_sprite(0, 0);

    position = origin;
    from = origin;
    to = *steps[0];

    life_bar->set_camera(camera);
    life_bar->set_visible(false);
    life_bar->set_z_order(-1000); // needs to be lower than the lowest towers's z order
    hit_sfx = bn::sound_items::hit;
    death_sfx = bn::sound_items::death;

    display_memory_left();
}

Enemy::~Enemy()
{
    log("enemy deleted", id);
}

void Enemy::on_tick(Game *game)
{
    if (current_step == steps_number)
    {
        // remove
        game->get_player()->on_hit(this);
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
        life_bar->set_visible(true);
    }

    life_bar->set_position(bn::fixed_point(position.x(), position.y() - 8));
    sprite->set_position(position);
    sprite->set_z_order(1000); // needs to be higher than the highest towers's z order

    if (animation.has_value())
    {
        animation->update();
    }
}

bool Enemy::is_dead()
{
    return dead;
}

bool Enemy::is_active()
{
    return !is_dead();
}

bn::fixed_point Enemy::get_position()
{
    return position;
}

void Enemy::hit(bn::fixed dmg, Player *player)
{
    life -= (dmg - armor);

    if (life <= 0)
    {
        log("enemy dead, id:", id);
        player->on_target_killed(this);
        dead = true;
        play_sfx(death_sfx.value(), 1);
        return;
    }

    play_sfx(hit_sfx.value(), 1);

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
        sprite->dimensions().width(),
        sprite->dimensions().height());
}

bn::fixed Enemy::get_reward()
{
    return reward;
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
}

void Enemy::set_animation_left_walk()
{
}

void Enemy::set_animation_down_walk()
{
}

void Enemy::set_animation_up_walk()
{
}

void Enemy::update_animation()
{
    bn::fixed deg = degrees_atan2(from, to);

    if (deg == bad_deg || !sprite.has_value())
    {
        // ugly early return to avoid finding real source of bugs
        return;
    }

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
