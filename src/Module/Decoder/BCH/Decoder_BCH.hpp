#ifndef DECODER_BCH
#define DECODER_BCH

#include <vector>

#include "Tools/Code/BCH/BCH_Polynomial_Generator.hpp"

#include "../Decoder_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_BCH : public Decoder_SIHO_HIHO<B,R>
{
private:
	std::vector<std::vector<int>> elp;
	std::vector<int> discrepancy;
	std::vector<int> l;
	std::vector<int> u_lu;
	std::vector<int> s;
	std::vector<int> loc;
	std::vector<int> reg;

protected:
	const int m;               // order of the Galois Field
	const int t;               // correction power
	const int d;               // minimum distance of the code (d=2t+1))

	const std::vector<int>& alpha_to; // log table of GF(2**m)
	const std::vector<int>& index_of; // antilog table of GF(2**m)

	std::vector<B> YH_N;       // hard decision input vector

public:
	Decoder_BCH(const int& K, const int& N, const tools::BCH_Polynomial_Generator &GF, const int n_frames = 1,
	            const std::string name = "Decoder_BCH");
	virtual ~Decoder_BCH();

protected:
	void _decode        (      B *Y_N                            );
	void _decode_hiho   (const B *Y_N, B *V_K, const int frame_id);
	void _decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id);
	void _decode_siho   (const R *Y_N, B *V_K, const int frame_id);
	void _decode_siho_cw(const R *Y_N, B *V_N, const int frame_id);
};
}
}

#endif /* DECODER_BCH */
