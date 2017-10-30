#ifndef ENCODER_LDPC_DVBS2_CONSTANTS_HPP_
#define ENCODER_LDPC_DVBS2_CONSTANTS_HPP_

#include <vector>

namespace aff3ct
{
namespace module
{
class dvbs2_values
{
public:
	std::vector<int> EncValues;
	int N_LDPC;
	int K_LDPC;
	int NmK_LDPC;
	int M_LDPC;
	int Q_LDPC;
	int N_LINES;
};

}
}

#include "Encoder_LDPC_DVBS2_constants_64800.hpp"
#include "Encoder_LDPC_DVBS2_constants_16200.hpp"

#endif
