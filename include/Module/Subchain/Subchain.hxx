#include "Module/Subchain/Subchain.hpp"

namespace aff3ct
{
namespace module
{

Task& Subchain
::operator[](const sch::tsk t)
{
	return Module::operator[]((size_t)t);
}

}
}