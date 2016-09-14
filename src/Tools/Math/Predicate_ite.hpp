#ifndef PREDICATE_ITE_HPP
#define PREDICATE_ITE_HPP

#include <cassert>

#include "Predicate.hpp"

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

#endif /* PREDICATE_ITE_HPP */
