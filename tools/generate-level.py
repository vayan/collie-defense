from cgitb import enable
import json
import numpy
import os
from PIL import Image

from ldtkpy.api import ldtk_json_from_dict

LTDK_PROJECT_PATH = "ldtk/levels.ldtk"
BASE_LTDK_PROJECT_PATH = os.path.dirname(LTDK_PROJECT_PATH)

print("\nLoading LTdk JSON..")
content = None
with open(LTDK_PROJECT_PATH) as f:
    content = ldtk_json_from_dict(json.load(f))
print("Done!\n")


# for images with more than 16 colors we need to use the same palette
# https://gvaliente.github.io/butano/faq.html#faq_multiple_8bpp_objects
print("\nLoading palette..")
palette_img = Image.open("tools/palette.bmp")
print("Done!\n")


def parse_entities(entities):
    print("Parsing entities values...")
    _entities_tiles_values = {}

    for entity in entities:
        _entities_tiles_values.update({entity.identifier: entity.uid})
    print("Done!\n")
    return _entities_tiles_values


def parse_grid_tiles_values(layers):
    print("Parsing grid tiles values...")
    _grid_tiles_values = {}

    for layer in layers:
        if layer.type != "IntGrid":
            continue
        for int_grid_value in layer.int_grid_values:
            _grid_tiles_values.update({int_grid_value.identifier: int_grid_value.value})

    print("Done!\n")
    return _grid_tiles_values


def generate_world_file(_grid_tiles_values, _entities_tiles_values, _levels, _enums):
    print("Generating world header file...")

    enums = ""

    for enum_def in _enums:
        enum_val = ""
        for k, v in enumerate(enum_def.values):
            enum_val = f"{enum_val}{v.id} = {k},\n\t\t"

        enums = f"""
    {enums}
    enum class {enum_def.identifier} {{
        {enum_val}
    }};
    """

    tiles_int_def = ""
    for k, v in _grid_tiles_values.items():
        tiles_int_def = f"{tiles_int_def}{k} = {v},\n\t\t"

    entities_def = ""
    for k, v in _entities_tiles_values.items():
        entities_def = f"{entities_def}{k} = {v},\n\t\t"

    enums = f"""
    {enums}
    enum class GridTileType {{
        empty = 0,
        {tiles_int_def}
    }};

    enum class EntityType {{
        {entities_def}
    }};
    """

    intgrid_map_filename = f"./include/generated/world_config.h"
    os.makedirs(os.path.dirname(intgrid_map_filename), exist_ok=True)
    with open(intgrid_map_filename, "w") as intgridkey_file:
        intgridkey_file.write(
            f"""
#ifndef COLLIE_DEFENCE_GBA_LEVEL_WORLDCONFIG_H
#define COLLIE_DEFENCE_GBA_LEVEL_WORLDCONFIG_H

#include "bn_fixed.h"

namespace cd {{
    const bn::fixed number_of_levels = {len(_levels)};
    {enums}
}}

#endif
"""
        )
    print("Done!\n")


def parse_levels(_levels):
    print("Parsing all levels...")
    parsed_levels = []
    for index, raw_level in enumerate(_levels):
        music = None
        enabled = True
        for field in raw_level.field_instances:
            if field.identifier == "enabled":
                enabled = field.value

            if field.identifier == "music" and field.value != None:
                music, _ = os.path.splitext(os.path.basename(field.value))

        parsed_level = {
            "int_identifier": index,
            "identifier": raw_level.identifier,
            "width": raw_level.px_wid,
            "height": raw_level.px_hei,
            "int_grid_width": int(raw_level.px_wid / content.world_grid_width),
            "int_grid_height": int(raw_level.px_hei / content.world_grid_height),
            "music": music,
            "enabled": enabled,
        }
        for layer_instance in raw_level.layer_instances:
            if (
                layer_instance.type == "IntGrid"
                and layer_instance.identifier == "IntGrid"
            ):
                parsed_level.update({"int_grid": layer_instance.int_grid_csv})
                pass
            if layer_instance.type == "Entities":
                entities = []
                for entity_instance in layer_instance.entity_instances:
                    fields = {
                        "points": [],
                        "enemy": None,
                        "rate": 1,
                        "waveNumber": 0,
                        "waveDuration": 5,
                    }
                    for field_instance in entity_instance.field_instances:
                        if field_instance.type == "LocalEnum.EnemyType":
                            fields["enemy"] = field_instance.value
                        if field_instance.type == "Float":
                            fields["rate"] = field_instance.value
                        if field_instance.type == "Array<Point>":
                            fields["points"] = field_instance.value
                        if field_instance.identifier == "WaveNumber":
                            fields["waveNumber"] = field_instance.value
                        if field_instance.identifier == "WaveDuration":
                            fields["waveDuration"] = field_instance.value
                    entities.append(
                        {
                            "type": entity_instance.identifier,
                            "location": entity_instance.px,
                            "fields": fields,
                        }
                    )
                parsed_level.update({"entities": entities})
                pass
        if parsed_level["enabled"]:
            parsed_levels.append(parsed_level)
    print("Done!\n")
    return parsed_levels


def generate_level_intgrid_file(_levels):
    intgrid_string = []
    headers = ""
    intgridfilename = f"./include/generated/levels_intgrid.h"
    os.makedirs(os.path.dirname(intgridfilename), exist_ok=True)

    for _level in _levels:
        headers = (
            f'{headers}\n#include "generated/levels/level_{_level["int_identifier"]}.h"'
        )
        intgrid_string.append(f'&level_{_level["int_identifier"]}')

    with open(intgridfilename, "w") as cpp_file:
        cpp_file.write(
            f"""
#ifndef COLLIE_DEFENCE_GBA_LEVEL_INTGRID_H
#define COLLIE_DEFENCE_GBA_LEVEL_INTGRID_H
{headers}
#include "level.h"

namespace cd {{
    BN_DATA_EWRAM static Level* levels[] = {str(intgrid_string).replace('[', '{').replace(']', '}').replace("'", '')};
}}

#endif
"""
        )


def convert_palette(img):
    img = img.quantize(
        colors=256,
        palette=palette_img,
    )

    # get the image palette as RGB tuples: [(r,g,b) (r,g,b), etc..]
    palette = [tuple(img.getpalette()[i * 3 : (i + 1) * 3]) for i in range(256)]

    # for now every color stay at their original place
    new_mapping = list(range(256))

    try:
        # find the first occurence of the transparent color (black for now)
        # and put it in index 0 (Butano read the color at index 0 to set
        # the transparent color)
        transparent_color_index = palette.index((0, 0, 0))
        new_mapping[0], new_mapping[transparent_color_index] = (
            new_mapping[transparent_color_index],
            new_mapping[0],
        )

    except:
        # .index() will raise an error if there's no black in the palette
        print("no transparent color in this level")
        os.exit(1)

    img = img.remap_palette(new_mapping)

    return img


def import_level_png(_levels):
    print("Converting all levels PNG to BMP/JSON...")
    num_columns = 2
    column_spacing = 30
    x_base_offset = 28
    y_base_offset = 28

    merged_image = Image.new("RGB", (256, 512), color="#a8c23d")

    x_offset = x_base_offset
    y_offset = y_base_offset

    for i, _level in enumerate(_levels):
        zfill_id = str(_level["int_identifier"]).zfill(4)
        os.makedirs("./graphics/generated/levels", exist_ok=True)

        image_png = f'{BASE_LTDK_PROJECT_PATH}/levels/png/{_level["identifier"]}.png'
        img = Image.open(image_png)

        newimg = img.convert(
            mode="RGB",
        )
        newimg = convert_palette(newimg)

        alpha = newimg.split()[-1]
        bbox = alpha.getbbox()
        image_to_merge = newimg.crop(bbox)
        level_resized = (image_to_merge.width // 3, image_to_merge.height // 3)
        image_to_merge = image_to_merge.resize(
            (image_to_merge.width // 15, image_to_merge.height // 15)
        )

        image_to_merge = image_to_merge.resize(level_resized, resample=Image.NEAREST)
        column_index = i % num_columns
        if column_index > 0:
            x_offset += level_resized[0] + column_spacing
        if column_index == 0 and i > 0:
            y_offset += level_resized[1] + y_base_offset
            x_offset = x_base_offset
        merged_image.paste(image_to_merge, (x_offset, y_offset))

        newimg.save(f"./graphics/generated/levels/levels_{zfill_id}.bmp")

        json_filename = f"./graphics/generated/levels/levels_{zfill_id}.json"
        with open(json_filename, "w") as json_file:
            json_file.write(
                f"""{{
  "type": "regular_bg"
}}"""
            )

    merged_image = merged_image.convert(
        mode="RGB",
    )
    merged_image = convert_palette(merged_image)
    merged_image.save(f"./graphics/generated/levels/all_levels.bmp")
    json_filename = f"./graphics/generated/levels/all_levels.json"
    with open(json_filename, "w") as json_file:
        json_file.write(
            f"""{{
                "type": "regular_bg"
            }}"""
        )

    print("Done!\n")


entities_tiles_values = parse_entities(content.defs.entities)

grid_tiles_values = parse_grid_tiles_values(content.defs.layers)

levels = parse_levels(content.levels)

generate_world_file(
    grid_tiles_values, entities_tiles_values, levels, content.defs.enums
)

generate_level_intgrid_file(levels)

import_level_png(levels)

print("Generating all levels header files...")
for level_index, level in enumerate(levels):
    zfill_id = str(level["int_identifier"]).zfill(4)
    level_name = f'level_{level["int_identifier"]}'
    filename = f"./include/generated/levels/{level_name}.h"
    os.makedirs(os.path.dirname(filename), exist_ok=True)

    music_declar = "bn::optional<bn::music_item>()"

    if level["music"]:
        music_declar = f"bn::music_items::{level['music']}"

    entities_var_list = []
    entities_var_declar = ""

    for index, entity in enumerate(level["entities"]):
        id = level_index * 1000 + index
        var_name = f'level_{zfill_id}_entity_{entity["type"]}_{index}'

        path_points_var_declr = ""
        path_points_var_list = []

        for index, path_point in enumerate(entity["fields"]["points"]):
            path_point_var_name = f"{var_name}_path_point_{index}"
            path_points_var_declr = f'{path_points_var_declr}\nconst bn::fixed_point {path_point_var_name} = bn::fixed_point({(path_point["cx"] * 2) - (level["width"] / 2)}, {(path_point["cy"] * 2) - (level["height"] / 2)});'
            path_points_var_list.append(f"&{path_point_var_name}")

        path_coords_value = ""
        path_coords_var_name = "nullptr"
        if len(path_points_var_list) > 0:
            path_coords_value = f"""BN_DATA_EWRAM static const bn::fixed_point *{var_name}_path_coords[] = {str(path_points_var_list).replace('[', '{').replace(']', '}').replace("'", '')};"""
            path_coords_var_name = f"{var_name}_path_coords"

        entities_var_list.append(f"&{var_name}")
        entities_var_declar = f"""

{path_points_var_declr}
{path_coords_value}
{entities_var_declar}\nconst Entity {var_name} = Entity(
    {id},
    EntityType::{entity["type"]},
    {entity["location"][0]},
    {entity["location"][1]},
    {path_coords_var_name},
    {len(path_points_var_list)},
    EnemyType::{entity["fields"]["enemy"]},
    {entity["fields"]["rate"]},
    {entity["fields"]["waveNumber"]},
    {entity["fields"]["waveDuration"]}
);
"""

    with open(filename, "w") as cpp_level:
        cpp_level.write(
            f"""
#ifndef COLLIE_DEFENCE_GBA_LEVEL_{zfill_id}_H
#define COLLIE_DEFENCE_GBA_LEVEL_{zfill_id}_H

#include "bn_regular_bg_items_levels_{zfill_id}.h"
#include "bn_utility.h"
#include "bn_string.h"
#include "bn_fixed_point.h"
#include "level.h"
#include "entity.h"
#include "generated/world_config.h"

namespace cd {{
    {entities_var_declar}
    BN_DATA_EWRAM static const Entity* entities_{level['int_identifier']}[] = {str(entities_var_list).replace('[', '{').replace(']', '}').replace("'", '')};
    const int int_grid_{level['int_identifier']}[] = {str(level['int_grid']).replace('[', '{').replace(']', '}')};
    BN_DATA_EWRAM static Level level_{level['int_identifier']} = Level(
        bn::regular_bg_items::levels_{zfill_id},
        int_grid_{level['int_identifier']},
        entities_{level['int_identifier']},
        {len(entities_var_list)},
        {music_declar}
    );
}}
#endif
"""
        )
print("Done!")
