/*!
 * \file
 * \brief Class tools::Predicate.
 */
#ifndef PREDICATE_HPP
#define PREDICATE_HPP

#include "Tools/Interface/Interface_reset.hpp"

namespace aff3ct
{
namespace tools
{
/*!
 * \class Predicate
 *
 * \brief A predicate.
 */
class Predicate : public Interface_reset
{
public:
	/*!
	 * \brief virtual Destructor.
	 */
	virtual ~Predicate() = default;

	/*!
	 * \brief Evaluates the predicate.
	 *
	 * \return true if the predicate meets the condition(s).
	 */
	virtual bool operator()() = 0;

	/*!
	 * \brief Resets the predicate state.
	 */
	virtual void reset() = 0;
};
}
}

#endif /* PREDICATE_HPP */
