#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace FFS
{

struct Target
{
	std::string bin;
	std::vector<std::string> srcs;

	static std::vector<Target>
	read_targets(std::ifstream &targets_file);

	void
	compile()
	const;
};

void
build();

};