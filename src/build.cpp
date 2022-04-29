#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "../lib/toml/toml.hpp"
#include "build.hpp"
#include "config.hpp"
#include "util.hpp"

FFS::Target::Target(const std::string &bin, const toml::array &srcs)
	: bin(bin)
{
	for (const auto &elem : srcs)
	{
		if (!elem.is_string())
		{
			util::die("Error: src must be a list of "
				"strings\n`%s`: [target.%s.src]\n",
				CONFIG_FILENAME, bin.c_str());
		}

		this->srcs.push_back(elem.as_string()->get());
	}
}

std::vector<FFS::Target>
FFS::Target::read_targets()
{
	auto config = read_config_file();
	std::vector<Target> targets;
	auto *targets_table = config["targets"].as_table();

	for (auto &&[ target, settings ] : *targets_table)
	{
		std::string bin = std::string(target.str());

		if (!settings.is_table())
		{
			util::die("Error: targets must be a table\n"
				"`%s`: [targets.%s]\n",
				CONFIG_FILENAME, target.str());
		}

		auto table = *settings.as_table();

		if (!table.contains("src"))
		{
			util::die("Error: targets must contain a \"src\" "
				"list\n`%s`: [targets.%s.src]\n",
				CONFIG_FILENAME, target.str());
		}

		if (!table["src"].is_array())
		{
			util::die("Error: targets.%s.src must be a list\n"
				"`%s`: [targets.%s.src]\n",
				target.str(), CONFIG_FILENAME, target.str());
		}

		targets.push_back({ bin, *table["src"].as_array() });
	}

	return targets;
}

static const char *default_CXX = "c++";
static const char *CXX = nullptr;

static const char *
get_CXX()
{
	if (CXX != nullptr)
	{
		return CXX;
	}

	if (getenv("CXX") != nullptr)
	{
		CXX = getenv("CXX");
		return CXX;
	}

	CXX = default_CXX;
	return CXX;
}

static std::string
compile_to_obj(const std::string &src)
{
	// Compute the hash of the source file. This will be used for the file
	// name of the object file.

	std::string obj = "obj/" + FFS::util::file_hash(src) + ".o";

	// If the object file is already compiled, we can just return.

	if (std::filesystem::exists(obj))
	{
		return obj;
	}

	// Compile the source file.

	std::string compiler = get_CXX();
	std::string cmd = compiler + " -std=c++20 -c " + src + " -o " + obj;
	printf("Compiling %s to %s...\n", src.c_str(), obj.c_str());
	system(cmd.c_str());

	return obj;
}

void
FFS::Target::compile()
const
{
	// Go through each source file and compile it.

	std::vector<std::string> obj_file_names;

	for (const std::string &src : srcs)
	{
		obj_file_names.push_back(compile_to_obj("src/" + src));
	}

	// Compile the target.

	std::string cmd = get_CXX();
	cmd += " ";

	for (const std::string &obj_file_name : obj_file_names)
	{
		cmd += obj_file_name + " ";
	}

	cmd += "-o bin/" + bin;
	printf("Linking bin/%s...\n", bin.c_str());
	system(cmd.c_str());
}

void
FFS::build()
{
	// Make sure the `bin` directory exists.
	// This directory is used to store the compiled executables.

	std::filesystem::create_directory("bin");

	// Make sure the `obj` directory exists.
	// This directory is used to store the compiled objects.

	std::filesystem::create_directory("obj");

	std::vector<Target> targets = Target::read_targets();

	// Iterate over all targets and compile them.

	for (const Target &target : targets)
	{
		target.compile();
	}
}