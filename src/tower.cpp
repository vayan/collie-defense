#include "tower.h"
#include "level.h"

using namespace cd;

Tower::Tower(
    bn::camera_ptr _camera,
    bn::fixed_point _position) : camera(_camera),
                                 position(_position)
{
    sprite = bn::sprite_items::tower.create_sprite(0, 0);

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
    if (bullets.full())
    {
        return;
    }
    last_fire_timer = bn::timer();
    bullets.emplace_back(camera, position, target);
}

void Tower::on_tick(Level *level)
{
    sprite.value().set_position(position);

    if (bn::keypad::a_pressed())
    {
        auto path = level->get_paths()->begin();
        auto enemy = path->get_enemies()->begin();
        fire(enemy);
    }

    for (auto bullet = bullets.begin(); bullet != bullets.end();)
    {
        bullet->on_tick(level);

        if (bullet->to_be_destroyed())
        {
            bullets.erase(bullet);
            return;
        }

        ++bullet;
    }
}

bn::vector<Bullet, 2> *Tower::get_bullets()
{
    return &bullets;
}
