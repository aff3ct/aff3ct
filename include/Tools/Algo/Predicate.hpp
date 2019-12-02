/*!
 * \file
 * \brief Class tools::Predicate.
 */
#ifndef PREDICATE_HPP
#define PREDICATE_HPP

#include "Tools/Interface/Interface_reset.hpp"
#ifndef _MSC_VER
#include "Tools/Interface/Interface_clone.hpp"
#endif

namespace aff3ct
{
namespace tools
{
/*!
 * \class Predicate
 *
 * \brief A predicate.
 */
#ifdef _MSC_VER
class Predicate : public Interface_reset
#else
class Predicate : public Interface_reset, Interface_clone
#endif
{
public:
	/*!
	 * \brief virtual Destructor.
	 */
	virtual ~Predicate() = default;

	virtual Predicate* clone() const = 0;

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
