/*!
 * \file
 * \brief A predicate.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PREDICATE_HPP
#define PREDICATE_HPP

namespace aff3ct
{
namespace tools
{
/*!
 * \class Predicate
 *
 * \brief A predicate.
 */
class Predicate
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
