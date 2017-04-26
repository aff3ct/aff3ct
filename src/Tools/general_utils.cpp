#include <sstream>

#include "general_utils.h"

std::vector<std::string> aff3ct::tools::string_split(const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
		elems.push_back(std::move(item));

	return elems;
}
