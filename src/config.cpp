#include "config.hpp"

toml::parse_result
FFS::read_config_file()
{
	return toml::parse_file(CONFIG_FILENAME);
}