#ifndef DECODER_RS_STD
#define DECODER_RS_STD

#include <vector>

#include "Tools/Code/RS/RS_polynomial_generator.hpp"
#include "Module/Decoder/RS/Decoder_RS.hpp"

/* source : http://www.eccpage.com/  */

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_RS_std : public Decoder_RS<B,R>
{
public:
	using typename Decoder_RS<B,R>::S; // symbol to represent data

private:
	const int t2;

	std::vector<std::vector<int>> elp;
	std::vector<int> discrepancy;
	std::vector<int> l;
	std::vector<int> u_lu;
	std::vector<int> s;
	std::vector<int> loc;
	std::vector<int> root;
	std::vector<int> reg;
	std::vector<int> z;
	std::vector<int> err;

public:
	Decoder_RS_std(const int& K, const int& N, const tools::RS_polynomial_generator &GF, const int n_frames = 1);
	virtual ~Decoder_RS_std() = default;

protected:
	virtual void _decode(S *Y_N, const int frame_id);
};
}
}

#endif /* DECODER_RS_STD */
