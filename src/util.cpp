#include "util.hpp"

bool
FFS::util::str_eq(const char *str, ...)
{
	va_list va;
	va_start(va, str);

	while (true)
	{
		const char *arg = va_arg(va, const char *);

		if (arg == nullptr)
		{
			va_end(va);
			return false;
		}

		if (strcmp(str, arg) == 0)
		{
			va_end(va);
			return true;
		}
	}
}

bool
FFS::util::should_ignore_line(const std::string &line)
{
	// Comments.

	if (line.starts_with("#"))
	{
		return true;
	}

	// Empty lines and lines consisting of only spaces and tabs.

	for (char c : line)
	{
		if (c != ' ' && c != '\t')
		{
			return false;
		}
	}

	return true;
}