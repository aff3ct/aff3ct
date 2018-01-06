#include "Argument_range.hpp"

using namespace aff3ct;
using namespace tools;

template <>
template <>
const char* Set_range<const char*>::template adapt<std::string>(const std::string& val) const
{
	return val.c_str();
}