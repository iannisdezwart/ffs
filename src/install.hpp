#pragma once

#include <fstream>
#include <string>
#include <utility>

#include "../lib/toml/toml.hpp"
#include "config.hpp"
#include "util.hpp"

namespace FFS
{

struct Dependency
{
	std::string name;
	std::string url;

	Dependency(const std::string &name, const toml::node &node)
		: name(name)
	{
		if (node.is_string())
		{
			url = node.as_string()->get();
		}
		else if (node.is_table())
		{
			const toml::table &table = *node.as_table();
			util::die("Error: tabular dependencies are not yet "
				"implemented\n`%s`: [dependencies.%s]\n",
				CONFIG_FILENAME, name.c_str());
		}
		else
		{
			util::die("Error: dependency must be a string or "
				"table\n`%s`: [dependencies.%s]\n",
				CONFIG_FILENAME, name.c_str());
		}
	}

	static std::vector<Dependency>
	read_dependencies();

	friend bool
	operator==(const Dependency &a, const Dependency &b);

	bool
	installed()
	const;

	std::string
	root_dir()
	const;

	void
	install()
	const;
};

void
install();

};

template <>
struct std::hash<FFS::Dependency>
{
	size_t
	operator()(const FFS::Dependency &dep)
	const;
};