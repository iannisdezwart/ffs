#include <cstring>
#include <stdarg.h>

#include "build.hpp"
#include "install.hpp"

#define IS(str, ...) str_eq(str, __VA_ARGS__)

bool
str_eq(const char *str, ...)
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

int
main(int argc, char **argv)
{
	if (argc < 2 || IS(argv[1], "build"))
	{
		FFS::build();
		return 0;
	}

	if (IS(argv[1], "install", "i"))
	{
		FFS::install();
		return 0;
	}
}