#include "debug.h"

using namespace cd;

const int IWRAM_BYTES = 32'768, EWRAM_BYTES = 262'144;

void cd::display_memory_left()
{
    const int iwramUsedPercent =
        (bn::fixed(bn::memory::used_static_iwram() + bn::memory::used_stack_iwram()) / IWRAM_BYTES * 100)
            .integer();
    const int ewramUsedPercent =
        (bn::fixed(EWRAM_BYTES - bn::memory::available_alloc_ewram()) / EWRAM_BYTES * 100).integer();
    const int iwramFree = IWRAM_BYTES - bn::memory::used_static_iwram() - bn::memory::used_stack_iwram();
    const int ewramFree = bn::memory::available_alloc_ewram();

    BN_LOG(bn::format<20>("IW: {}% {}", iwramUsedPercent, iwramFree));
    BN_LOG(bn::format<20>("EW: {}% {}", ewramUsedPercent, ewramFree));
}

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

void cd::log(bn::string<50> label, save_data data)
{
    bn::string<255> scores = "";
    for (int i = 0; i < number_of_levels; i += 1)
    {
        scores = scores + bn::format<60>("{}\t| {}\n", i, data.score_per_level[i]);
    }

    BN_LOG(bn::format<255>(
        "{}\nstory:\n level: {}\n money: {}\n life: {}\nstage\t| score\n{}",
        label,
        data.latest_story_level,
        data.story_money,
        data.story_life,
        scores));
}
