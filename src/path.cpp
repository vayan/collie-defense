#include "path.h"

namespace cd
{
    Path::Path(
        bn::fixed_point _from,
        bn::camera_ptr _camera,
        bn::fixed _fire_pause_sec,
        bn::fixed_point **_steps,
        bn::fixed _steps_number,
        bn::fixed _wave_order,
        bn::fixed _wave_duration_sec) : from(_from),
                                        camera(_camera),
                                        fire_pause_sec(_fire_pause_sec),
                                        steps(_steps),
                                        steps_number(_steps_number),
                                        wave_order(_wave_order),
                                        wave_duration_sec(_wave_duration_sec)

    {
        last_fire_timer = bn::timer();
    }

    Path::~Path()
    {
    }

    void Path::on_tick(Level *level, Player *player)
    {
        if (!first_tick_timer.has_value())
        {
            first_tick_timer = bn::timer();
        }
        else if (first_tick_timer.value().elapsed_ticks() > (one_sec_in_ticks.integer() * wave_duration_sec.integer()))
        {
            is_finished = true;
        }

        if (!is_finished && last_fire_timer.value().elapsed_ticks() > (one_sec_in_ticks * fire_pause_sec) && !enemies.full())
        {
            last_fire_timer = bn::timer();
            enemies.emplace_back(camera, from, steps, steps_number);
        }

        for (Enemy &enemy : enemies)
        {
            enemy.on_tick(level, player);

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

    bn::fixed Path::get_wave_order()
    {
        return wave_order;
    }

    bool Path::to_be_removed()
    {
        return is_finished && enemies.empty();
    }
}
