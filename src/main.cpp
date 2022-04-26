#include "build.hpp"
#include "install.hpp"
#include "help.hpp"
#include "util.hpp"

int
main(int argc, char **argv)
{
	if (argc < 2 || FFS::util::is_any_of(argv[1], "build"))
	{
		FFS::build();
		return 0;
	}

	if (FFS::util::is_any_of(argv[1], "help", "h", "--help", "-h"))
	{
		FFS::help(argc, argv);
		return 0;
	}

	if (FFS::util::is_any_of(argv[1], "install", "i"))
	{
		FFS::install();
		return 0;
	}

	fprintf(stderr, "Unknown command: `%s`.\nRun `%s help` for help",
		argv[1], argv[0]);
	return 1;
}