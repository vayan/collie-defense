#include "sheep.h"

using namespace cd;

Sheep::Sheep(
    const bn::fixed _id,
    bn::camera_ptr _camera,
    const bn::fixed_point _origin,
    const bn::fixed_point **_steps,
    const bn::fixed _steps_number) : id(_id),
                                     camera(_camera),
                                     origin(_origin),
                                     steps(_steps),
                                     steps_number(_steps_number)
{
    sprite = bn::sprite_items::sheep.create_sprite(origin);
    position = origin;
    from = origin;
    to = *steps[0];
}

Sheep::~Sheep()
{
}

void Sheep::on_tick()
{
    if (current_step == steps_number)
    {
        current_step = -1;
        from = *steps[steps_number.integer() - 1];
        to = origin;
        progress = 0;
        animation.reset();
    }

    bn::fixed dist = distance(from, to) * 100;
    if (dist != 0)
    {
        progress += speed.safe_division(dist);
    }

    position = lerp_points(from, to, progress);

    if (progress >= 1)
    {
        current_step += 1;
        from = to;
        to = *steps[current_step.integer()];
        progress = 0;
        animation.reset();
    }

    if (!animation.has_value())
    {
        update_animation();
    }

    sprite->set_position(position);

    if (position.y().integer() % 8 == 0)
    {
        sprite->set_z_order(((position.y() * -1) + 1).integer());
    }

    if (animation.has_value())
    {
        animation->update();
    }
}

void Sheep::set_animation_right_walk()
{
    if (animation.has_value() && animation->graphics_indexes().front() == 0)
    {
        return;
    }
    sprite->set_horizontal_flip(false);
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7);
}

void Sheep::set_animation_left_walk()
{
    set_animation_right_walk();
    sprite.value()
        .set_horizontal_flip(true);
}

void Sheep::set_animation_down_walk()
{
    if (animation.has_value() && animation->graphics_indexes().front() == 16)
    {
        return;
    }
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        16, 17, 18, 19, 20, 21, 22, 23);
}

void Sheep::set_animation_up_walk()
{
    if (animation.has_value() && animation->graphics_indexes().front() == 32)
    {
        return;
    }
    animation = bn::create_sprite_animate_action_forever(
        sprite.value(),
        8,
        bn::sprite_items::sheep.tiles_item(),
        32, 33, 34, 35, 36, 37, 38, 39);
}

void Sheep::update_animation()
{
    bn::fixed deg = degrees_atan2(from, to);

    if (deg == bad_deg)
    {
        return;
    }

    if (deg > 45 && deg < 135)
    {
        set_animation_up_walk();
    }
    else if (deg < -45 && deg > -135)
    {
        set_animation_down_walk();
    }
    else if (deg <= 45 && deg >= -45)
    {
        set_animation_left_walk();
    }
    else
    {
        set_animation_right_walk();
    }
}
