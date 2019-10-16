#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Predicate_ite.hpp"

namespace aff3ct
{
namespace tools
{
Predicate_ite::Predicate_ite(const int n_ite)
: n_ite(n_ite), cur_ite(0)
{
	if (n_ite <= 0)
	{
		std::stringstream message;
		message << "'n_ite' has to be equal or greater than 0 ('n_ite' = " << n_ite << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

bool Predicate_ite::operator()()
{
	const bool predicate = cur_ite >= n_ite;
	cur_ite++;
	return predicate;
}

void Predicate_ite::reset()
{
	cur_ite = 0;
}
}
}