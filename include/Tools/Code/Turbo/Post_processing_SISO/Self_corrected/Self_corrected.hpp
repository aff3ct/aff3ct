/*!
 * \file
 * \brief Class tools::Self_corrected.
 */
#ifndef SELF_CORRECTED_HPP
#define SELF_CORRECTED_HPP

#include <vector>
#include <mipp.h>

#include "Tools/Code/Turbo/Post_processing_SISO/Post_processing_SISO.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Self_corrected : public Post_processing_SISO<B,R>
{
private:
	const int K;
	const int m;
	const int M;

	std::vector<mipp::vector<R>> ext_nat; // extrinsic coming from the SISO working in the natural domain. Data in the interleaved order
	std::vector<mipp::vector<R>> ext_int; // extrinsic coming from the SISO working in the interleaved domain. Data in the natural order
	mipp::vector<B> osc_nat;
	mipp::vector<B> osc_int;
	mipp::vector<B> previously_corrected_nat;
	mipp::vector<B> previously_corrected_int;

public:
	Self_corrected(const int K, const int n_ite, const int m = 4, const int M = -1);

	virtual ~Self_corrected() = default;

	bool siso_n(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext,
	                  mipp::vector<B>& s);

	bool siso_i(const int ite,
	            const mipp::vector<R>& sys,
	                  mipp::vector<R>& ext);

	virtual Self_corrected<B,R>* clone() const;

	virtual void set_n_frames(const size_t n_frames);

private:
	void collect (const int ite, const mipp::vector<R> &cur_ext, std::vector<mipp::vector<R>> &ext_hist);
	void calc_osc(const int ite, const std::vector<mipp::vector<R>> &ext_hist, mipp::vector<B> &osc);
	void correct (const mipp::vector<B> &osc, mipp::vector<B> &prev_corr, mipp::vector<R> &ext);
};
}
}

#endif /* SELF_CORRECTED_HPP */
