#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

#include "config.hpp"
#include "install.hpp"
#include "util.hpp"

std::vector<FFS::Dependency>
FFS::Dependency::read_dependencies()
{
	// Read the dependencies from the config file.

	auto config = read_config_file();
	std::vector<Dependency> deps;
	auto *deps_table = config["dependencies"].as_table();

	for (auto &&[ dep_key, settings ] : *deps_table)
	{
		std::string dep_name = std::string(dep_key.str());
		Dependency dep = { dep_name, settings };
		deps.push_back(dep);
	}

	return deps;
}

bool
FFS::operator==(const FFS::Dependency &a, const FFS::Dependency &b)
{
	return a.name == b.name;
}

bool
FFS::Dependency::installed()
const
{
	return std::filesystem::exists("lib/" + name);
}

std::string
FFS::Dependency::root_dir()
const
{
	return "lib/" + name;
}

void
FFS::Dependency::install()
const
{
	printf("Installing %s\n", name.c_str());

	// Install the dependency.

	std::string cmd = "git clone " + url + " \"" + root_dir() + "\"";
	printf("Installing %s...\n", cmd.c_str());
	system(cmd.c_str());
}

void
FFS::install()
{
	// Make sure the "lib" directory exists.
	// This directory is used to store all dependencies.

	std::filesystem::create_directory("lib");

	std::vector<Dependency> deps = Dependency::read_dependencies();

	// Install each dependency.

	for (const auto &dep : deps)
	{
		// Skip dependencies that are already installed.

		if (dep.installed())
		{
			continue;
		}

		// Install the dependency. The name of the dependency is added
		// to the log file to indicate it has been installed.

		dep.install();
	}
}