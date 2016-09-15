#ifndef QUANTIZER_TRICKY_HPP_
#define QUANTIZER_TRICKY_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Quantizer.hpp"

template <typename R, typename Q>
class Quantizer_tricky : public Quantizer<R,Q>
{
private:
	const int val_max;
	const int val_min;
	R delta_inv;
	const R& sigma;

public:
	Quantizer_tricky(const int N,
	                 const R& sigma,
	                 const int n_frames = 1, 
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N,
	                 const short& saturation_pos, 
	                 const R& sigma, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N, 
	                 const float min_max, 
	                 const R& sigma, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	Quantizer_tricky(const int N,
	                 const float min_max, 
	                 const short& saturation_pos, 
	                 const R& sigma, 
	                 const int n_frames = 1,
	                 const std::string name = "Quantizer_tricky");
	virtual ~Quantizer_tricky();

	void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2);

};

#endif /* QUANTIZER_TRICKY_HPP_ */

