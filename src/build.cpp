#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "build.hpp"
#include "util.hpp"

std::vector<FFS::Target>
FFS::Target::read_targets(std::ifstream &targets_file)
{
	std::vector<Target> targets;
	std::string line;
	Target target;
	bool read_target = false;

	while (std::getline(targets_file, line))
	{
		// Targets are formatted as follows:
		/*
			<bin1>:
				- <src>
				- <src>
				...

			<bin2>:
				- <src>
				- <src>
				...

			...
		*/

		// Ignore empty lines and comments.

		if (util::should_ignore_line(line))
		{
			continue;
		}

		// If we are on the first level of indentation, we are starting
		// a new target.

		if (!util::is_space(line[0]))
		{
			// If we have already read a target, we need to add it
			// to the vector.

			if (read_target)
			{
				targets.push_back(target);
			}

			// Create a new target.

			read_target = true;
			target = {};
			target.bin = line.substr(0, line.find(':'));
		}
		else
		{
			// If we are on the second level of indentation, we are
			// listing the source files.
			// Add each source file to the target.

			util::trim(line);

			if (!line.starts_with('-'))
			{
				std::cerr << "Invalid target line: " << line
					<< std::endl;
				exit(1);
			}

			line = line.substr(1);
			util::trim(line);

			target.srcs.push_back(line);
		}
	}

	// Add the last target to the vector.

	if (read_target)
	{
		targets.push_back(target);
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

	std::string file_name = "obj/" + FFS::util::file_hash(src) + ".obj";

	// If the object file is already compiled, we can just return.

	if (std::filesystem::exists(file_name))
	{
		return file_name;
	}

	// Compile the source file.

	std::string compiler = get_CXX();
	std::string cmd = compiler + " -std=c++20 -c " + src + " -o " + file_name;
	system(cmd.c_str());

	return file_name;
}

void
FFS::Target::compile()
const
{
	// Go through each source file and compile it.

	std::vector<std::string> obj_file_names;

	for (const std::string &src : srcs)
	{
		obj_file_names.push_back(compile_to_obj(src));
	}

	// Compile the target.

	std::string cmd = get_CXX();
	cmd += " ";

	for (const std::string &obj_file_name : obj_file_names)
	{
		cmd += obj_file_name + " ";
	}

	cmd += "-o bin/" + bin;
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

	// Read the `targets` file.
	// This file contains a list of targets that can be compiled.

	std::ifstream targets_file("targets");

	if (!targets_file.is_open())
	{
		std::cerr << "The `targets` file does not exist." << std::endl
			<< "Please create it." << std::endl;
		return;
	}

	std::vector<Target> targets = Target::read_targets(targets_file);

	// Iterate over all targets and compile them.

	for (const Target &target : targets)
	{
		target.compile();
	}
}