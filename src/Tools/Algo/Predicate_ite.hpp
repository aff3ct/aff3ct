/*!
 * \file
 * \brief A predicate: return true if it has been called n times or more.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */

#ifndef PREDICATE_ITE_HPP
#define PREDICATE_ITE_HPP

#include <cassert>

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
	Predicate_ite(const int n_ite) 
	: n_ite(n_ite), cur_ite(0)
	{
		assert(n_ite >= 0);
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
