#include "path.h"

namespace cd
{
    Path::Path(
        bn::fixed_point _from,
        bn::camera_ptr _camera,
        bn::fixed _fire_pause_sec,
        bn::fixed_point **_steps,
        bn::fixed _steps_number) : from(_from),
                                   camera(_camera),
                                   fire_pause_sec(_fire_pause_sec),
                                   steps(_steps),
                                   steps_number(_steps_number)
    {
        last_fire_timer = bn::timer();
    }

    Path::~Path()
    {
    }

    void Path::on_tick(Level *level)
    {
        if (last_fire_timer.value().elapsed_ticks() > (one_sec_in_ticks * fire_pause_sec) && !enemies.full())
        {
            last_fire_timer = bn::timer();
            enemies.emplace_back(camera, from, steps, steps_number);
        }

        for (Enemy &enemy : enemies)
        {
            enemy.on_tick(level);

            if (enemy.is_dead())
            {
                enemies.erase(&enemy);
            }
        }
    }

    bn::vector<Enemy, 20> *Path::get_enemies()
    {
        return &enemies;
    }
}
