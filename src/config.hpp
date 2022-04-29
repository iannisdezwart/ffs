#pragma once

#include "../lib/toml/toml.hpp"

namespace FFS
{

static const constexpr char *CONFIG_FILENAME = "ffs.toml";
static const constexpr char *INSTALLED_DEPS_FILENAME = "lib/.installed";

toml::parse_result
read_config_file();

}