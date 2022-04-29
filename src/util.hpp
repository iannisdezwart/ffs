#pragma once

#include <cstring>
#include <stdarg.h>
#include <string>

namespace FFS::util
{

bool
str_eq(const char *str, ...);

template <typename... Strings>
bool
is_any_of(const char *str, Strings... strings)
{
	return str_eq(str, strings..., nullptr);
}

bool
should_ignore_line(const std::string &line);

bool
is_space(char c);

void
trim(std::string &str);

size_t
str_hash(char *buf, size_t size);

std::string
file_hash(const std::string &file_path);

template <typename... Args>
[[ noreturn ]]
void
die(const char *fmt, Args... args)
{
	fprintf(stderr, fmt, args...);
	exit(1);
}

}