#include <cassert>
#include <ios>

#include "Reporter.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

void Reporter
::init()
{
}

const std::vector<Reporter::group_t>& Reporter
::get_groups() const
{
	return this->cols_groups;
}