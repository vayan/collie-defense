#include "entity.h"

using namespace cd;

Entity::Entity(
    const bn::fixed _id,
    EntityType _type,
    bn::fixed x,
    bn::fixed y,
    bn::fixed_point **_arr_points_1,
    bn::fixed _arr_points_1_size,
    bn::string<10> _string_10_1,
    bn::fixed _number_1,
    bn::fixed _number_2,
    bn::fixed _number_3) : id(_id),
                           type(_type),
                           arr_points_1(_arr_points_1),
                           arr_points_1_size(_arr_points_1_size),
                           string_10_1(_string_10_1),
                           number_1(_number_1),
                           number_2(_number_2),
                           number_3(_number_3)
{
    position = bn::fixed_point(x, y);
}

Entity::~Entity()
{
}

EntityType Entity::get_type() const
{
    return type;
}

bn::fixed_point Entity::get_position() const
{
    return position;
}

bn::fixed Entity::get_number_1() const
{
    return number_1;
}

bn::fixed Entity::get_number_2() const
{
    return number_2;
}

bn::fixed Entity::get_number_3() const
{
    return number_3;
}

bn::fixed_point **Entity::get_arr_points_1() const
{
    return arr_points_1;
}

bn::fixed Entity::get_arr_points_1_size() const
{
    return arr_points_1_size;
}

bn::fixed Entity::get_id() const
{
    return id;
}
