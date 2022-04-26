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

}