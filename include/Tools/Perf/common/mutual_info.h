/*!
 * \file
 * \brief Functions to compute mutual information.
 */
#ifndef MUTUAL_INFO_H__
#define MUTUAL_INFO_H__

namespace aff3ct
{
namespace tools
{
/*
 * compute the mutal information between 'ref' and 'llr' of length 'size'
 * with the histogram method
 */
template <typename B, typename R>
R mutual_info_histo_seq(const B* ref, const R* llr, const unsigned size);

/*
 * compute the mutal information between 'ref' and 'llr' of length 'size'
 * with the histogram method
 * operations are optimized with MIPP except on 8 or 16 bits and for AVX architecture
 * that call mutual_info_histo_seq
 */
template <typename B, typename R>
R mutual_info_histo(const B* ref, const R* llr, const unsigned size);
}
}
#endif // MUTUAL_INFO_H__
