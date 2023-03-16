#include "wave.h"
#include "game.h"

namespace cd
{
    Wave::Wave(
        const bn::fixed _id,
        const bn::fixed_point _from,
        bn::camera_ptr _camera,
        const bn::fixed _fire_pause_sec,
        const bn::fixed_point **_steps,
        const bn::fixed _steps_number,
        const bn::fixed _wave_order,
        const bn::fixed _wave_duration_sec,
        const EnemyType _enemy_type) : id(_id),
                                       from(_from),
                                       camera(_camera),
                                       fire_pause_sec(_fire_pause_sec),
                                       steps(_steps),
                                       steps_number(_steps_number),
                                       wave_order(_wave_order),
                                       wave_duration_sec(_wave_duration_sec),
                                       enemy_type(_enemy_type)

    {
        log("wave created", id);

        rand = bn::random();

        for (bn::fixed i = 0; i < id; i += 1)
        {
            rand->update();
        }
    }

    Wave::~Wave()
    {
        for (Enemy *enemy : enemies)
        {
            delete enemy;
        }
        log("wave deleted", id);
    }

    void Wave::on_tick(Game *game)
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
                enemies.push_back(new EnemyTiger(generate_enemy_id(), camera, from, steps, steps_number));
                break;
            default:
                break;
            }
        }

        erase_if(
            enemies,
            [=, this](Enemy *enemy)
            {
                enemy->on_tick(game);

                if (enemy->is_dead())
                {
                    game->get_current_level()->clear_bullets_with_invalid_target();
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

    const bn::fixed Wave::get_wave_order()
    {
        return wave_order;
    }

    bool Wave::to_be_removed()
    {
        return is_finished && enemies.empty();
    }

    bn::fixed Wave::generate_enemy_id()
    {
        return id + rand->get_fixed(id, id + 10000).ceil_integer();
    }
}
