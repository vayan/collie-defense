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
    if (
        bullets.full() ||
        (last_fire_timer.has_value() &&
         last_fire_timer.value().elapsed_ticks() < (one_sec_in_ticks * 1)))
    {
        return;
    }
    last_fire_timer = bn::timer();
    bullets.emplace_back(camera, position, target);
}

void Tower::on_tick(Level *level, Player *player)
{
    sprite.value().set_position(position);

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

            bool is_inside = delta_x * delta_x + delta_y * delta_y < aggro_radius_px * aggro_radius_px;
            if (is_inside)
            {
                fire(&enemy);
            }
        }
    }
}

bn::vector<Bullet, 1> *Tower::get_bullets()
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
    }
}
