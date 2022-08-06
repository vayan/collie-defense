#include "entity.h"

using namespace cd;

Entity::Entity(
    const bn::fixed _id,
    EntityType _type,
    bn::fixed x, bn::fixed y,
    const bn::pair<bn::string<10>, bn::fixed_point> **_coords,
    const bn::pair<bn::string<10>, bn::string<string_metadata_max_size>> **_strings,
    bn::fixed _coords_size,
    bn::fixed _strings_size) : id(_id), type(_type), coords(_coords), strings(_strings), coords_size(_coords_size), strings_size(_strings_size)
{
    position = bn::fixed_point(x, y);
}

Entity::~Entity()
{
}
