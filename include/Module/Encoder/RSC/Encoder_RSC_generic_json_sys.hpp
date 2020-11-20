/*!
 * \file
 * \brief Class module::Encoder_RSC_generic_json_sys.
 */
#ifndef ENCODER_RSC_GENERIC_JSON_SYS_HPP_
#define ENCODER_RSC_GENERIC_JSON_SYS_HPP_

#include <vector>
#include <iostream>

#include "Module/Encoder/RSC/Encoder_RSC_generic_sys.hpp"

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
	                             const bool buffered_encoding = true, std::vector<int> poly = std::vector<int>({5,7}),
	                             std::ostream &stream = std::cout);
	virtual ~Encoder_RSC_generic_json_sys() = default;

	std::vector<std::vector<int>> get_trellis();

	virtual void set_n_frames(const size_t n_frames);

protected:
	int inner_encode(const int bit_sys, int &state);
};
}
}

#endif // ENCODER_RSC_GENERIC_JSON_SYS_HPP_
