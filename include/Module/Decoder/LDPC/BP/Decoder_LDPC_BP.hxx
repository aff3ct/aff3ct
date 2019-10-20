#include "Tools/Code/LDPC/Syndrome/LDPC_syndrome.hpp"
#include "Module/Decoder/LDPC/BP/Decoder_LDPC_BP.hpp"

namespace aff3ct
{
namespace module
{
template <typename R>
bool Decoder_LDPC_BP
::check_syndrome_soft(const R* Y_N)
{
	if (this->enable_syndrome)
	{
		const auto syndrome = tools::LDPC_syndrome::check_soft(Y_N, this->H);
		this->cur_syndrome_depth = syndrome ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
		return syndrome && (this->cur_syndrome_depth == 0);
	}
	else
		return false;
}

template <typename B>
bool Decoder_LDPC_BP
::check_syndrome_hard(const B* V_N)
{
	if (this->enable_syndrome)
	{
		const auto syndrome = tools::LDPC_syndrome::check_hard(V_N, this->H);
		this->cur_syndrome_depth = syndrome ? (this->cur_syndrome_depth +1) % this->syndrome_depth : 0;
		return syndrome && (this->cur_syndrome_depth == 0);
	}
	else
		return false;
}
}
}
