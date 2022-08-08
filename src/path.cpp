#include "path.h"

BN_DATA_EWRAM bn::vector<cd::Enemy, 40> enemies;

namespace cd
{
    Path::Path(
        bn::fixed_point _from,
        bn::camera_ptr _camera,
        bn::fixed _fire_rate,
        bn::fixed_point **_steps,
        bn::fixed _steps_number) : from(_from),
                                   camera(_camera),
                                   fire_rate(_fire_rate),
                                   steps(_steps),
                                   steps_number(_steps_number)
    {
        last_fire_timer = bn::timer();
    }

    Path::~Path()
    {
    }

    void Path::on_tick()
    {
        if (last_fire_timer.value().elapsed_ticks() > 167800 * fire_rate && !enemies.full())
        {
            BN_LOG("fire!!!");
            last_fire_timer = bn::timer();
            enemies.emplace_back(camera, from, steps, steps_number);
        }

        for (cd::Enemy &enemy : enemies)
        {
            enemy.on_tick();

            // TODO delete dead enemy
        }
    }
}
