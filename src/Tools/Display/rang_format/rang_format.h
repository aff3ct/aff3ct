#ifndef RANG_FORMAT_H_
#define RANG_FORMAT_H_

#include <string>
#include <iostream>
#include <rang.hpp>

namespace rang
{

enum class format {
	error   = 0,
	warning = 1,
	info    = 2,
	reset   = 3
};

std::ostream& operator<<(std::ostream& os, const format f);

void format_on_each_line(std::ostream& os, const std::string& str, const format f);

}

#endif /* RANG_FORMAT_H_ */
