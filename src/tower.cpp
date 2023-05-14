#include "tower.h"
#include "game.h"
#include "level.h"
#include "player.h"

using namespace cd;

Tower::Tower(bn::camera_ptr _camera, bn::fixed_point _position) : camera(_camera),
                                                                  position(_position)
{
    display_memory_left();
    fire_sfx = bn::sound_items::ranged_shot;
}

void Tower::fire(Target *target)
{
    if (bullets.full() || (frame_elapsed_since_last_fire / frame_per_sec) < fire_rate_per_sec)
    {
        return;
    }
    fire_sfx->play(0.2);
    frame_elapsed_since_last_fire = 0;
    bullets.push_back(new Bullet(
        camera,
        bn::fixed_point(
            position.x() + bullet_start_position_offset.x(),
            position.y() + bullet_start_position_offset.y()),
        target,
        bullet_speed,
        damage,
        bullet_sprite.value()));
}

Tower::~Tower()
{
    for (Bullet *bullet : bullets)
    {
        delete bullet;
    }
    log("tower deleted");
}

void Tower::on_tick(Game *game)
{
    frame_elapsed_since_last_fire += 1;
    set_position(position);

    erase_if(
        bullets,
        [=, this](Bullet *bullet)
        {
            bullet->on_tick(game);

            if (bullet->to_be_destroyed())
            {
                delete bullet;
                return true;
            }

            return false;
        });

    for (Wave *wave : *game->get_current_level()->get_waves())
    {
        for (Enemy *enemy : *wave->get_enemies())
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

bn::vector<Bullet *, 2> *Tower::get_bullets()
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
        sprite->set_z_order((y * -1).integer());
    }
}

void Tower::set_position(bn::fixed_point pos)
{
    set_position(pos.x(), pos.y());
}

void Tower::clear_bullets_with_invalid_target()
{
    erase_if(
        bullets,
        [=, this](Bullet *bullet)
        {
            if (bullet->to_be_destroyed())
            {
                delete bullet;
                return true;
            }

            return false;
        });
}

TowerType Tower::get_type()
{
    return type;
}

bn::optional<bn::sprite_ptr> Tower::get_sprite()
{
    return sprite;
}

bn::fixed_point Tower::get_position()
{
    return position;
}
