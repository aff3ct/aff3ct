/*!
 * \file
 * \brief Class module::Decoder_BCH_std.
 */
#ifndef DECODER_BCH_STD
#define DECODER_BCH_STD

#include <vector>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"
#include "Module/Decoder/BCH/Decoder_BCH.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH_std : public Decoder_BCH<B,R>
{
private :
	const int t2;

protected:
	std::vector<B> YH_N; // hard decision input vector
	std::vector<std::vector<int>> elp;
	std::vector<int> discrepancy;
	std::vector<int> l;
	std::vector<int> u_lu;
	std::vector<int> s;
	std::vector<int> loc;
	std::vector<int> reg;

	const int m;               // order of the Galois Field
	const int d;               // minimum distance of the code (d=2t+1))

	const std::vector<B>& alpha_to; // log table of GF(2**m)
	const std::vector<B>& index_of; // antilog table of GF(2**m)


public:
	Decoder_BCH_std(const int& K, const int& N, const tools::BCH_polynomial_generator<B> &GF);
	virtual ~Decoder_BCH_std() = default;
	virtual Decoder_BCH_std<B,R>* clone() const;

protected:
	virtual int _decode        (      B *Y_N,                      const size_t frame_id);
	virtual int _decode_hiho   (const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
	virtual int _decode_siho   (const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id);
	virtual int _decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id);
};
}
}

#endif /* DECODER_BCH_STD */
