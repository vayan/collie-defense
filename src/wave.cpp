#include "wave.h"

namespace cd
{
    Wave::Wave(
        bn::fixed _id,
        bn::fixed_point _from,
        bn::camera_ptr _camera,
        bn::fixed _fire_pause_sec,
        bn::fixed_point **_steps,
        bn::fixed _steps_number,
        bn::fixed _wave_order,
        bn::fixed _wave_duration_sec,
        EnemyType _enemy_type) : id(_id),
                                 from(_from),
                                 camera(_camera),
                                 fire_pause_sec(_fire_pause_sec),
                                 steps(_steps),
                                 steps_number(_steps_number),
                                 wave_order(_wave_order),
                                 wave_duration_sec(_wave_duration_sec),
                                 enemy_type(_enemy_type)

    {
        frame_elapsed_since_last_fire = fire_pause_sec;
        rand = bn::random();

        for (bn::fixed i = 0; i < id; i += 1)
        {
            rand.value().update();
        }
    }

    Wave::~Wave()
    {
    }

    void Wave::on_tick(Level *level, Player *player)
    {
        frame_elapsed_since_last_fire += 1;
        frame_elapsed_since_create += 1;

        if ((frame_elapsed_since_create / frame_per_sec) > wave_duration_sec)
        {
            is_finished = true;
        }

        if (!is_finished && (frame_elapsed_since_last_fire / frame_per_sec) > fire_pause_sec && !enemies.full())
        {
            frame_elapsed_since_last_fire = 0;
            switch (enemy_type)
            {
            case EnemyType::Bear:
                enemies.push_back(new EnemyBear(generate_enemy_id(), camera, from, steps, steps_number));
                break;
            case EnemyType::Fox:
                enemies.push_back(new EnemyFox(generate_enemy_id(), camera, from, steps, steps_number));
                break;
            case EnemyType::Tiger:
                // todo
                break;
            default:
                break;
            }
        }

        erase_if(
            enemies,
            [=](Enemy *enemy)
            {
                enemy->on_tick(level, player);

                if (enemy->is_dead())
                {
                    delete enemy;
                    return true;
                }
                return false;
            });
    }

    bn::vector<Enemy *, 20> *Wave::get_enemies()
    {
        return &enemies;
    }

    bn::fixed Wave::get_wave_order()
    {
        return wave_order;
    }

    bool Wave::to_be_removed()
    {
        return is_finished && enemies.empty();
    }

    bn::fixed Wave::generate_enemy_id()
    {
        return id + rand.value().get_fixed(id, id + 10000).ceil_integer();
    }
}
