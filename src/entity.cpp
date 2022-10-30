#include "entity.h"

using namespace cd;

Entity::Entity(
    const bn::fixed _id,
    const EntityType _type,
    const bn::fixed x,
    const bn::fixed y,
    const bn::fixed_point **_arr_points_1,
    const bn::fixed _arr_points_1_size,
    const EnemyType _enemy_type,
    const bn::fixed _number_1,
    const bn::fixed _number_2,
    const bn::fixed _number_3) : id(_id),
                                 type(_type),
                                 arr_points_1(_arr_points_1),
                                 arr_points_1_size(_arr_points_1_size),
                                 enemy_type(_enemy_type),
                                 number_1(_number_1),
                                 number_2(_number_2),
                                 number_3(_number_3)
{
    position = bn::fixed_point(x, y);
}

Entity::~Entity()
{
}

const EntityType Entity::get_type() const
{
    return type;
}

bn::fixed_point Entity::get_position() const
{
    return position;
}

const bn::fixed Entity::get_number_1() const
{
    return number_1;
}

const bn::fixed Entity::get_number_2() const
{
    return number_2;
}

const bn::fixed Entity::get_number_3() const
{
    return number_3;
}

const bn::fixed_point **Entity::get_arr_points_1() const
{
    return arr_points_1;
}

const bn::fixed Entity::get_arr_points_1_size() const
{
    return arr_points_1_size;
}

const bn::fixed Entity::get_id() const
{
    return id;
}

const EnemyType Entity::get_enemy_type() const
{
    return enemy_type;
}
