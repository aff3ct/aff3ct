#include "Module/Sleeper/Sleeper.hpp"

namespace aff3ct
{
namespace module
{

Task& Sleeper
::operator[](const slp::tsk t)
{
	return Module::operator[]((size_t)t);
}

Socket& Sleeper
::operator[](const slp::sck::sleep s)
{
	return Module::operator[]((size_t)slp::tsk::sleep)[(size_t)s];
}

}
}
