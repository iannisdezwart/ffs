#pragma once

#include "../lib/toml/toml.hpp"

namespace FFS
{

static const constexpr char *CONFIG_FILENAME = "ffs.toml";

toml::parse_result
read_config_file();

}