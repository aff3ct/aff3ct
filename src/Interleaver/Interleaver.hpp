#ifndef INTERLEAVER_HPP_
#define INTERLEAVER_HPP_

#include <string>
#include <cassert>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename T>
class Interleaver_interface // please use Interleaver<T> for inheritance (instead of Interleaver_interface<T>)
{
protected:
	mipp::vector<T> pi;     //   interleaver lookup table
	mipp::vector<T> pi_inv; // deinterleaver lookup table

	int n_frames;

	std::string name; // module name
	
	virtual void gen_lookup_tables() = 0; // to implement

public:
	Interleaver_interface(const int size, const int n_frames = 1, const std::string name = "Interleaver_interface") 
	: pi(size), pi_inv(size), n_frames(n_frames), name(name) {};
	virtual ~Interleaver_interface() {};

	mipp::vector<T> get_lookup_table        () const { return pi;     }
	mipp::vector<T> get_lookup_table_inverse() const { return pi_inv; }

	template <typename D>
	inline void interleave(const mipp::vector<D> &natural_vec, 
	                             mipp::vector<D> &interleaved_vec, 
	                       const bool             frame_reordering = false) const
	{
		this->_interleave(natural_vec, interleaved_vec, pi, frame_reordering);
	}

	template <typename D>
	inline void deinterleave(const mipp::vector<D> &interleaved_vec, 
	                               mipp::vector<D> &natural_vec, 
	                         const bool             frame_reordering = false) const
	{ 
		this->_interleave(interleaved_vec, natural_vec, pi_inv, frame_reordering);
	}

	virtual void set_n_frames(const int n_frames)
	{
		assert(n_frames > 0);
		this->n_frames = n_frames;
	}

	bool same_lookup_table(Interleaver_interface<T> &interleaver)
	{
		if (interleaver.pi.size() != this->pi.size())
			return false;

		if (interleaver.pi_inv.size() != this->pi_inv.size())
			return false;

		for (auto i = 0; i < (int)this->pi.size(); i++)
			if (this->pi[i] != interleaver.pi[i])
				return false;

		for (auto i = 0; i < (int)this->pi_inv.size(); i++)
			if (this->pi_inv[i] != interleaver.pi_inv[i])
				return false;

		return true;
	}

	void rename(const std::string name)
	{
		this->name = name;
	}

private:
	template <typename D>
	inline void _interleave(const mipp::vector<D> &in_vec, 
	                              mipp::vector<D> &out_vec,
	                        const mipp::vector<T> &lookup_table,
	                        const bool             frame_reordering = false) const
	{
		assert(in_vec.size() == out_vec.size());
		assert(in_vec.size() >= lookup_table.size() * n_frames);
		const auto frame_size = (int)lookup_table.size();

		if (frame_reordering)
		{
			// vectorized interleaving
			if (n_frames == mipp::nElReg<D>())
			{
				for (auto i = 0; i < frame_size; i++)
					mipp::store<D>(&out_vec[i * mipp::nElReg<D>()], 
					               mipp::load<D>(&in_vec[lookup_table[i] * mipp::nElReg<D>()]));
			}
			else
			{
				if ((n_frames == 4 && typeid(D) == typeid(signed char)) && // partial vectorized interleaving
				    (in_vec.size() >= (unsigned)(frame_size * n_frames + mipp::nElReg<D>()))) 
				{
					const signed char mask[32] = { 0, 0, 0, 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					                              -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
					const auto r_mask = mipp::set<signed char>(mask);

					for (auto i = 0; i < frame_size; i++)
					{
						const auto off1 =              i  * n_frames;
						const auto off2 = lookup_table[i] * n_frames;

						auto r_ld = mipp::loadu<D>(&in_vec[off2]);
						auto r_st = mipp::loadu<D>(&in_vec[off1]);

						r_ld = mipp::andnb<D>(r_mask, r_ld);
						r_st = mipp::andb <D>(r_mask, r_st);
						r_st = mipp::xorb <D>(r_ld,   r_st);

						mipp::storeu<D>(&out_vec[off1], r_st);
					}
				}
				else // sequential interleaving
				{
					for (auto i = 0; i < frame_size; i++)
					{
						const auto off1 =              i  * n_frames;
						const auto off2 = lookup_table[i] * n_frames;
						for (auto f = 0; f < n_frames; f++)
							out_vec[off1 +f] = in_vec[off2 +f];
					}
				}
			}
		}
		else // sequential interleaving
		{
			// TODO: vectorize this code with the new AVX gather instruction
			for (auto f = 0; f < n_frames; f++)
			{
				const auto off = f * frame_size;
				for (auto i = 0; i < frame_size; i++)
					out_vec[off + i] = in_vec[off + lookup_table[i]];
			}
		}
	}
};

#include "SC_Interleaver.hpp"

#endif /* INTERLEAVER_HPP_ */