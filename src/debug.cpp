#include "debug.h"

using namespace cd;

void cd::log(bn::string<20> label, bn::fixed_point point)
{
    BN_LOG(bn::format<50>("{}: {},{}", label, point.x(), point.y()));
}

void cd::log(bn::string<50> message)
{
    BN_LOG(message);
}

void cd::log(bn::string<50> message, bn::fixed number)
{
    BN_LOG(bn::format<50>("{}: {}", message, number));
}
