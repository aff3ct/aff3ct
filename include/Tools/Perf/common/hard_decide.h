/*!
 * \file
 * \brief Functions to compute hard decision.
 */
#ifndef HARD_DECIDE_H_
#define HARD_DECIDE_H_

namespace aff3ct
{
namespace tools
{
/*
 * Take the hard decision on the array 'in' and fill 'out', both of length 'size'
 */
template <typename B = int, typename Q = float>
void hard_decide_seq(const Q *in, B *out, const unsigned size);

/*
 * Take the hard decision on the array 'in' and fill 'out', both of length 'size'
 * Check if 'in' values are unknown llr (tools::is_unknown_llr<Q>(in[i]))
 *    and then set them as unknown symbol tools::unknown_symbol_val<B>()
 */
template <typename B = int, typename Q = float>
void hard_decide_unk_seq(const Q *in, B *out, const unsigned size);

/*
 * Take the hard decision on the array 'in' and fill 'out', both of length 'size'
 * Operations are optimized with MIPP
 */
template <typename B = int, typename Q = float>
void hard_decide(const Q *in, B *out, const unsigned size);

/*
 * Take the hard decision on the array 'in' and fill 'out', both of length 'size'
 * Check if 'in' values are unknown llr (tools::is_unknown_llr<Q>(in[i]))
 *    and then set them as unknown symbol tools::unknown_symbol_val<B>().
 * Operations are optimized with MIPP
 */
template <typename B = int, typename Q = float>
void hard_decide_unk(const Q *in, B *out, const unsigned size);
}
}

#endif /* HARD_DECIDE_H_ */