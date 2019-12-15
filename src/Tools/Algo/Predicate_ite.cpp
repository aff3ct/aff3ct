#include "Tools/Algo/Predicate_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Predicate_ite* Predicate_ite
::clone() const
{
	return new Predicate_ite(*this);
}
