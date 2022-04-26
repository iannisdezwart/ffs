#pragma once

#include <fstream>
#include <string>
#include <utility>

namespace FFS
{

struct Dependency
{
	std::string name;
	std::string url;

	Dependency(const std::string &name, const std::string &url)
		: name(name), url(url) {}

	Dependency(const std::string &line)
		// Dependencies are formatted as "<name> <url>".
		: name(line.substr(0, line.find(' '))),
		  url(line.substr(line.find(' ') + 1)) {}

	friend bool
	operator==(const Dependency &a, const Dependency &b);

	std::string
	root_dir()
	const;

	void
	install(std::ofstream &deps_log_file_out)
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