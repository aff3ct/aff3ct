#include "Module/Subsequence/Subsequence.hpp"

namespace aff3ct
{
namespace module
{

Task& Subsequence
::operator[](const ssq::tsk t)
{
	return Module::operator[]((size_t)t);
}

}
}