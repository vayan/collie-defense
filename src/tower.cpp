#include "tower.h"
#include "level.h"
#include "player.h"

using namespace cd;

Tower::Tower(
    TowerType _type,
    bn::camera_ptr _camera,
    bn::fixed_point _position) : type(_type),
                                 camera(_camera),
                                 position(_position)
{
    sprite = get_sprite(type).create_sprite(0, 0);

    sprite.value()
        .set_position(position);
    sprite.value().set_camera(camera);
    sprite.value().set_visible(true);
}

Tower::~Tower()
{
}

void Tower::fire(Target *target)
{
    if (bullets.full() || (frame_elapsed_since_last_fire / frame_per_sec) < get_fire_rate_per_sec())
    {
        return;
    }
    frame_elapsed_since_last_fire = 0;
    bullets.emplace_back(camera, position, target, get_bullet_speed(), get_damage());
}

void Tower::on_tick(Level *level, Player *player)
{
    frame_elapsed_since_last_fire += 1;
    sprite.value()
        .set_position(position);

    for (Bullet &bullet : bullets)
    {
        bullet.on_tick(level, player);

        if (bullet.to_be_destroyed())
        {
            bullets.erase(&bullet);
        }
    }

    for (Path &path : *level->get_paths())
    {
        for (Enemy &enemy : *path.get_enemies())
        {
            bn::fixed delta_x = enemy.get_position().x() - position.x();
            bn::fixed delta_y = enemy.get_position().y() - position.y();

            bool is_inside = delta_x * delta_x + delta_y * delta_y < get_aggro_range() * get_aggro_range();
            if (is_inside)
            {
                fire(&enemy);
            }
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
        sprite.value().dimensions().width(),
        sprite.value().dimensions().height());
}

bn::sprite_item Tower::get_sprite(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return bn::sprite_items::tower_basic;
    case TowerType::AoE:
        return bn::sprite_items::tower_aoe;
    case TowerType::Sticky:
        return bn::sprite_items::tower_sticky;
    default:
        return bn::sprite_items::tower_basic;
    }
}

// I'll create real child classes at some point if I'm not lazy
bn::fixed Tower::get_aggro_range(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return 32;
    case TowerType::AoE:
        return 24;
    case TowerType::Sticky:
        return 24;
    default:
        return 10;
    }
}

bn::fixed Tower::get_aggro_range()
{
    return get_aggro_range(type);
}

bn::fixed Tower::get_damage(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return 10;
    case TowerType::AoE:
        return 20;
    case TowerType::Sticky:
        return 5;
    default:
        return 10;
    }
}

bn::fixed Tower::get_damage()
{
    return get_damage(type);
}

bn::fixed Tower::get_bullet_speed(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return 0.02;
    case TowerType::AoE:
        return 0.01;
    case TowerType::Sticky:
        return 0.04;
    default:
        return 0.02;
    }
}

bn::fixed Tower::get_bullet_speed()
{
    return get_bullet_speed(type);
}

bn::fixed Tower::get_cost(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return 10;
    case TowerType::AoE:
        return 50;
    case TowerType::Sticky:
        return 30;
    default:
        return 9999;
    }
}

bn::fixed Tower::get_cost()
{
    return get_cost(type);
}

bn::fixed Tower::get_fire_rate_per_sec(TowerType type)
{
    switch (type)
    {
    case TowerType::Basic:
        return 1;
    case TowerType::AoE:
        return 1;
    case TowerType::Sticky:
        return 1;
    default:
        return 1;
    }
}

bn::fixed Tower::get_fire_rate_per_sec()
{
    return get_fire_rate_per_sec(type);
}
