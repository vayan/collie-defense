import json
import os


CREDITS_JSON_FILE = "tools/credits.json"


def generate_credits():
    print("Start to generate credits data...")
    with open(CREDITS_JSON_FILE) as f:
        credits = json.load(f)

    final_list = []

    for category in credits:
        final_list.append(category["title"].upper())
        for person in category["names"]:
            final_list.append(f" {person}")

    intgrid_map_filename = f"./include/generated/credits.h"
    os.makedirs(os.path.dirname(intgrid_map_filename), exist_ok=True)
    with open(intgrid_map_filename, "w") as intgridkey_file:
        intgridkey_file.write(
            f"""
#pragma once

#include "bn_array.h"
#include "bn_string.h"

namespace cd {{
    inline const bn::array<bn::string<50>, {len(final_list)}> credits_lines = {str(final_list).replace('[', '{').replace(']', '}').replace("'", '"')};

}}
"""
        )
    print("Done!")


generate_credits()
