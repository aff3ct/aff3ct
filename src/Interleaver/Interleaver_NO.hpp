#ifndef INTERLEAVER_NO_HPP
#define	INTERLEAVER_NO_HPP

#include "Interleaver.hpp"

template <typename T>
class Interleaver_NO : public Interleaver<T>
{
public:
	Interleaver_NO(int size) : Interleaver<T>(size) { gen_lookup_tables(); }

protected:
	void gen_lookup_tables()
	{
		for (unsigned i = 0; i < this->pi.size(); i++)
			this->pi_inv[i] = this->pi[i] = i;
	}
};

#endif	/* INTERLEAVER_NO_HPP */