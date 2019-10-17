/*!
 * \file
 * \brief Class tools::Predicate_ite.
 */
#ifndef PREDICATE_ITE_HPP
#define PREDICATE_ITE_HPP

#include "Tools/Algo/Predicate.hpp"

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
	inline explicit Predicate_ite(const int n_ite);

	virtual ~Predicate_ite() = default;

	inline bool operator()();

	inline void reset();
};
}
}

#include "Tools/Algo/Predicate_ite.hxx"

#endif /* PREDICATE_ITE_HPP */
