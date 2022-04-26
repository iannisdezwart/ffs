#include <cstdio>

#include "help.hpp"
#include "util.hpp"

static void
print_main_help(char *argv0)
{
	printf("Welcome to the FFS build system.\n\n");
	printf("Usage:\n");
	printf("\t%s build                - Builds the binaries for your "
		"project.\n", argv0);
	printf("\t%s install              - Installs all dependencies for your "
		"project.\n", argv0);
	printf("\t%s install <name>       - Installs the given package for "
		"your project.\n", argv0);
	printf("\t%s install <name> <url> - Installs a git repository as a "
		"package.\n", argv0);
	printf("\t%s help                 - Prints this help message.\n",
		argv0);
}

void
FFS::help(int argc, char **argv)
{
	if (argc <= 2)
	{
		print_main_help(argv[0]);
		return;
	}

	if (util::is_any_of(argv[1], "build"))
	{
		printf("build\n");
		return;
	}
}