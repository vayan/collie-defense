#include "tower.h"
#include "game.h"
#include "level.h"
#include "player.h"

using namespace cd;

Tower::Tower(bn::camera_ptr _camera, bn::fixed_point _position) : camera(_camera),
                                                                  position(_position)
{
    display_memory_left();
}

void Tower::fire(Target *target)
{
    if (bullets.full() || (frame_elapsed_since_last_fire / frame_per_sec) < fire_rate_per_sec)
    {
        return;
    }
    frame_elapsed_since_last_fire = 0;
    bullets.emplace_back(
        camera,
        bn::fixed_point(
            position.x() + bullet_start_position_offset.x(),
            position.y() + bullet_start_position_offset.y()),
        target,
        bullet_speed,
        damage,
        bullet_sprite.value());
}

void Tower::on_tick(Game *game)
{
    frame_elapsed_since_last_fire += 1;
    set_position(position);

    for (Bullet &bullet : bullets)
    {
        bullet.on_tick(game);

        if (bullet.to_be_destroyed())
        {
            bullets.erase(&bullet);
        }
    }

    for (Wave &wave : *game->get_current_level()->get_waves())
    {
        for (Enemy *enemy : *wave.get_enemies())
        {

            bn::fixed delta_x = enemy->get_position().x() - position.x();
            bn::fixed delta_y = enemy->get_position().y() - position.y();

            bool is_inside = delta_x * delta_x + delta_y * delta_y < get_aggro_range() * get_aggro_range();
            if (is_inside)
            {
                update_animation(enemy);
                fire(enemy);
            }
        }
    }

    if (animation.has_value())
    {
        if (!animation->done())
        {
            animation->update();
        }
        else
        {
            animation.reset();
        }
    }
}

bn::vector<Bullet, 2> *Tower::get_bullets()
{
    return &bullets;
}

bn::fixed_rect Tower::get_hitbox()
{
    return bn::fixed_rect(
        position.x(),
        position.y(),
        sprite->dimensions().width(),
        sprite->dimensions().height());
}

bn::fixed Tower::get_aggro_range()
{
    return aggro_range;
}

bn::fixed Tower::get_cost()
{
    return cost;
}

void Tower::set_animation_shoot_right()
{
}

void Tower::set_animation_shoot_left()
{
}

void Tower::set_animation_shoot_down()
{
}

void Tower::set_animation_shoot_up()
{
}

void Tower::update_animation(Target *target)
{
    if (animation.has_value() && !animation->done())
    {
        return;
    }

    bn::fixed deg = degrees_atan2(position, target->get_position());

    if (deg == bad_deg)
    {
        return;
    }

    if (deg > 45 && deg < 135)
    {
        set_animation_shoot_up();
    }
    else if (deg < -45 && deg > -135)
    {
        set_animation_shoot_down();
    }
    else if (deg <= 45 && deg >= -45)
    {
        set_animation_shoot_left();
    }
    else
    {
        set_animation_shoot_right();
    }
}

void Tower::place()
{
}

void Tower::set_position(bn::fixed x, bn::fixed y)
{
    position.set_x(x);
    position.set_y(y);

    if (sprite.has_value())
    {
        sprite->set_position(position);
    }
}

void Tower::set_position(bn::fixed_point pos)
{
    set_position(pos.x(), pos.y());
}
