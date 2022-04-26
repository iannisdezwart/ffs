#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

#include "install.hpp"
#include "util.hpp"

size_t
std::hash<FFS::Dependency>::operator()(const FFS::Dependency &dep)
const
{
	return std::hash<std::string>()(dep.name);
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
FFS::Dependency::install(std::ofstream &deps_log_file_out)
const
{
	printf("Installing %s\n", name.c_str());

	// If the dependency root dir already exists, we will reinstall the
	// dependency.

	if (std::filesystem::exists(root_dir()))
	{
		std::filesystem::remove_all(root_dir());
	}

	deps_log_file_out << name << std::endl;
	std::string cmd = "git clone " + url + " \"" + root_dir() + "\"";
	system(cmd.c_str());
}

void
FFS::install()
{
	// Make sure the "lib" directory exists.
	// This directory is used to store all dependencies.

	std::filesystem::create_directory("lib");

	// Open the "dependencies" file. This file contains a list of
	// dependencies that need to be installed.

	std::ifstream deps_file("dependencies");

	// We will store the dependencies in a set, so we can check if there
	// are any duplicates. If there are duplicates, we will print an error.

	std::unordered_set<Dependency> deps_set;
	std::vector<Dependency> deps;

	// The ".dependencies-log" file stores a list of all installed
	// dependencies. This is used to prevent installing the same
	// dependency twice.

	std::ifstream deps_log_file_in(".dependencies-log");
	std::ofstream deps_log_file_out(".dependencies-log", std::ios::app);

	// We will store the installed dependencies in a set, so we can check
	// if a given dependency is already installed in O(1) time.

	std::unordered_set<Dependency> installed_deps;

	// If there are no dependencies to install, we can just return.

	if (!deps_file.is_open())
	{
		return;
	}

	// Read the dependencies file into the vector and check for duplicates.

	std::string line;

	while (std::getline(deps_file, line))
	{
		// Ignore empty lines and comments.

		if (util::should_ignore_line(line))
		{
			continue;
		}

		if (deps_set.find(line) != deps_set.end())
		{
			std::cerr << "Duplicate dependency: " << line
				<< std::endl;
			return;
		}

		deps_set.insert(line);
		deps.push_back(line);
	}

	// Read the already installed dependencies into the set.

	while (std::getline(deps_log_file_in, line))
	{
		// Ignore empty lines and comments.

		if (line.size() == 0 || line.starts_with("#"))
		{
			continue;
		}

		installed_deps.insert(line);
	}

	// Go through the dependencies and install each dependency that has not
	// been installed yet.

	for (const auto &dep : deps)
	{
		if (installed_deps.find(dep.name) != installed_deps.end())
		{
			// This dependency has already been installed.

			continue;
		}

		// Install the dependency. We add the name of the dependency
		// to the log file to indicate it has been installed.

		dep.install(deps_log_file_out);
	}
}