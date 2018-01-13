/*!
 * \file
 * \brief A predicate: return true if it has been called n times or more.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */

#ifndef PREDICATE_ITE_HPP
#define PREDICATE_ITE_HPP

#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Predicate.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Predicate_ite
 *
 * \brief A predicate: return true if it has been called n times or more.
 */
class Predicate_ite : public Predicate
{
private:
	const int n_ite;
	      int cur_ite;

public:
	explicit Predicate_ite(const int n_ite) 
	: n_ite(n_ite), cur_ite(0)
	{
		if (n_ite <= 0)
		{
			std::stringstream message;
			message << "'n_ite' has to be equal or greater than 0 ('n_ite' = " << n_ite << ").";
			throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Predicate_ite()
	{
	}

	bool operator()()
	{
		const bool predicate = cur_ite >= n_ite;
		cur_ite++;
		return predicate;
	}

	void reset()
	{
		cur_ite = 0;
	}
};
}
}

#endif /* PREDICATE_ITE_HPP */
