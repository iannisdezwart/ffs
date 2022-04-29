#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../lib/toml/toml.hpp"
#include "config.hpp"
#include "util.hpp"

namespace FFS
{

struct Target
{
	std::string bin;
	std::vector<std::string> srcs;

	Target() {}

	Target(const std::string &bin, const toml::array &srcs);

	static std::vector<Target>
	read_targets();

	void
	compile()
	const;
};

void
build();

};