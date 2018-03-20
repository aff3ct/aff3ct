#ifndef DECODER_BCH_STD
#define DECODER_BCH_STD

#include <vector>

#include "Tools/Code/BCH/BCH_polynomial_generator.hpp"

#include "../Decoder_BCH.hpp"

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
	std::vector<std::vector<int>> elp;
	std::vector<int> discrepancy;
	std::vector<int> l;
	std::vector<int> u_lu;
	std::vector<int> s;
	std::vector<int> loc;
	std::vector<int> reg;

	const int m;               // order of the Galois Field
	const int d;               // minimum distance of the code (d=2t+1))

	const std::vector<int>& alpha_to; // log table of GF(2**m)
	const std::vector<int>& index_of; // antilog table of GF(2**m)


public:
	Decoder_BCH_std(const int& K, const int& N, const tools::BCH_polynomial_generator &GF, const int n_frames = 1);
	virtual ~Decoder_BCH_std();

protected:
	virtual void _decode(B *Y_N, const int frame_id);
};
}
}

#endif /* DECODER_BCH_STD */
