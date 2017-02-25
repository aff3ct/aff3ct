#ifndef ENCODER_RSC_GENERIC_JSON_SYS_HPP_
#define ENCODER_RSC_GENERIC_JSON_SYS_HPP_

#include <vector>
#include <iostream>
#include "Tools/Perf/MIPP/mipp.h"

#include "Encoder_RSC_generic_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B>
class Encoder_RSC_generic_json_sys : public Encoder_RSC_generic_sys<B>
{
private:
	std::ostream &stream;
	int bit_counter;
	bool natural_domain;
	std::vector<int> poly;

public:
	Encoder_RSC_generic_json_sys(const int& K, const int& N, const int& n_frames = 1, 
	                             const bool buffered_encoding = true, std::vector<int> poly = {5,7},
	                             std::ostream &stream = std::cout,
	                             const std::string name = "Encoder_RSC_generic_json_sys");
	virtual ~Encoder_RSC_generic_json_sys() {}

	std::vector<std::vector<int>> get_trellis();

protected:
	int inner_encode(const int bit_sys, int &state);
};
}
}

#endif // ENCODER_RSC_GENERIC_JSON_SYS_HPP_
