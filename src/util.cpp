#include "util.hpp"
#include <fstream>
#include <sstream>

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

bool
FFS::util::is_space(char c)
{
	return c == ' ' || c == '\t';
}

void
FFS::util::trim(std::string &str)
{
	// Trim leading spaces.

	size_t start = 0;

	while (start < str.size() && is_space(str[start]))
	{
		start++;
	}

	// Trim trailing spaces.

	size_t end = str.size();

	while (end > start && is_space(str[end - 1]))
	{
		end--;
	}

	// Cut off the spaces.

	str = str.substr(start, end - start);
}

size_t
FFS::util::str_hash(char *buf, size_t size)
{
	// Random 64-bit prime to start the hash with.

	#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF
	size_t hash = 9552788573397850279UL;
	#else
	size_t hash = 2528669057UL;
	#endif

	// Loop over all characters in the string and add them to the hash.

	for (size_t i = 0; i < size; i += sizeof(size_t))
	{
		switch (size - i)
		{
		default:
		#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF
		case 8:
			hash ^= (size_t) buf[i + 7] << 56;
		case 7:
			hash ^= (size_t) buf[i + 6] << 48;
		case 6:
			hash ^= (size_t) buf[i + 5] << 40;
		case 5:
			hash ^= (size_t) buf[i + 4] << 32;
		#endif
		case 4:
			hash ^= (size_t) buf[i + 3] << 24;
		case 3:
			hash ^= (size_t) buf[i + 2] << 16;
		case 2:
			hash ^= (size_t) buf[i + 1] << 8;
		case 1:
			hash ^= (size_t) buf[i];
		}

		// Multiply by another random prime to randomise the hash.

		#if SIZE_MAX == 0xFFFFFFFFFFFFFFFF
		hash *= 12470784267989484971UL;
		#else
		hash *= 3100109627;
		#endif
	}

	return hash;
}

std::string
FFS::util::file_hash(const std::string &file_path)
{
	// Open the file.

	FILE *file = fopen(file_path.c_str(), "r");

	if (file == nullptr)
	{
		die("Could not open file \"%s\" for reading: %s\n",
			file_path.c_str(), strerror(errno));
	}

	char buf[4096];
	size_t hash = 0;
	ssize_t read;

	// Add all 4096 byte block hashes to the output hash.

	while (read = fread(buf, 1, sizeof(buf), file), read > 0)
	{
		hash ^= str_hash(buf, read);
	}

	// Clean up.

	fclose(file);

	// Convert the hash to a string.

	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}