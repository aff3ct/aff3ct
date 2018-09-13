#ifndef ENCODER_RSC_GENERIC_JSON_SYS_HPP_
#define ENCODER_RSC_GENERIC_JSON_SYS_HPP_

#include <vector>
#include <iostream>

#include "Encoder_RSC_generic_sys.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Encoder_RSC_generic_json_sys : public Encoder_RSC_generic_sys<B>
{
private:
	std::ostream &stream;
	int bit_counter;
	bool natural_domain;
	std::vector<int> poly;

public:
	Encoder_RSC_generic_json_sys(const int& K, const int& N,
	                             const bool buffered_encoding = true, std::vector<int> poly = {5,7},
	                             std::ostream &stream = std::cout, const int n_frames = 1);
	virtual ~Encoder_RSC_generic_json_sys() = default;

	std::vector<std::vector<int>> get_trellis();

protected:
	int inner_encode(const int bit_sys, int &state);
};
}
}

#endif // ENCODER_RSC_GENERIC_JSON_SYS_HPP_
