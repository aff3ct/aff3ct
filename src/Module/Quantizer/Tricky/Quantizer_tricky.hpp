#ifndef QUANTIZER_TRICKY_HPP_
#define QUANTIZER_TRICKY_HPP_

#include "../Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = float>
class Quantizer_tricky : public Quantizer<R,Q>
{
private:
	const int val_max;
	const int val_min;
	R delta_inv;

public:
	Quantizer_tricky(const int N,
	                 const int n_frames = 1, 
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N,
	                 const short& saturation_pos, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N, 
	                 const float min_max, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N,
	                 const float min_max, 
	                 const short& saturation_pos, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	virtual ~Quantizer_tricky();

	void process(const R *Y_N1, Q *Y_N2); using Quantizer<R,Q>::process;
};
}
}

#endif /* QUANTIZER_TRICKY_HPP_ */

