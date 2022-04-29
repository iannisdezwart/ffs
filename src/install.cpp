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
	// The installed dependencies file stores a list of all installed
	// dependencies. This is used to prevent installing the same
	// dependency twice.

	std::ifstream installed_deps_file(INSTALLED_DEPS_FILENAME);

	// We will store the installed dependencies in a set, so we can check
	// if a given dependency is already installed in O(1) time.

	std::unordered_set<std::string> installed_deps;

	// Read the installed dependencies file into the set.

	std::string line;

	while (std::getline(installed_deps_file, line))
	{
		installed_deps.insert(line);
	}

	// Now we will read the dependencies from the config file.

	auto config = read_config_file();
	std::vector<Dependency> deps;
	auto *deps_table = config["dependencies"].as_table();

	for (auto &&[ dep_key, settings ] : *deps_table)
	{
		std::string dep_name = std::string(dep_key.str());
		Dependency dep = { dep_name, settings };
		deps.push_back(dep);

		// If the dependency is already installed, we note this.

		dep.installed = true;
	}

	return deps;
}

bool
FFS::operator==(const FFS::Dependency &a, const FFS::Dependency &b)
{
	return a.name == b.name;
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

	// If the dependency root dir already exists, we will reinstall the
	// dependency.

	if (std::filesystem::exists(root_dir()))
	{
		std::filesystem::remove_all(root_dir());
	}

	// Add the dependency name to the installed dependencies file to
	// prevent it from being reinstalled.

	std::ofstream installed_deps_file(INSTALLED_DEPS_FILENAME);
	installed_deps_file << name << std::endl;

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

		if (dep.installed)
		{
			continue;
		}

		// Install the dependency. The name of the dependency is added
		// to the log file to indicate it has been installed.

		dep.install();
	}
}