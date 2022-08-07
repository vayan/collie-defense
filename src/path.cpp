#include "path.h"

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
        sprite = bn::sprite_items::sheep.create_sprite(0, 0);

        position = from;
        to = *steps[0];

        sprite.value()
            .set_position(from);
        sprite.value().set_camera(camera);
        sprite.value().set_visible(true);
    }

    Path::~Path()
    {
        sprite.value().remove_camera();
        sprite.value().set_visible(false);
        sprite.reset();
    }

    void Path::on_tick()
    {
        if (current_step == steps_number)
        {
            // remove
            return;
        }
        progress = progress + delta;

        position = lerp_points(from, to, progress);

        if (progress >= 1)
        {
            current_step += 1;
            from = to;
            to = *steps[current_step.integer()];
            progress = 0;
            // TODO check no more steps
        }

        sprite.value().set_position(position);
    }
}
