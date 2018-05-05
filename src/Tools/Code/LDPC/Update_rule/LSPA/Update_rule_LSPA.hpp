#ifndef UPDATE_RULE_LSPA_HPP
#define UPDATE_RULE_LSPA_HPP

#include <cassert>
#include <limits>
#include <cmath>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Update_rule_LSPA // Log Sum Product Algorithm
{
protected:
	std::vector<R> values;
	int sign;
	R   sum;
	int n_ite;
	int ite;

public:
	Update_rule_LSPA(const unsigned max_check_node_degree)
	: values(max_check_node_degree), sign(0), sum(1), n_ite(0), ite(0)
	{
		assert(max_check_node_degree > 0);
	}

	virtual ~Update_rule_LSPA()
	{
	}

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	inline void begin_decoding(const int n_ite)
	{
		this->n_ite = n_ite;
	}

	// FOR EACH iterations --------------------------------------------------------------------------------------- LOOP

		inline void begin_ite(const int ite)
		{
			this->ite = ite;
		}

		// FOR EACH check nodes ---------------------------------------------------------------------------------- LOOP

			// incoming values from the variable nodes into the check nodes
			inline void begin_check_node_in(const int CN_id, const int CN_degree)
			{
				assert(CN_degree <= values.size());

				this->sign = 0;
				this->sum  = 1;
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline void compute_check_node_in(const int VN_id, const R VN_value)
				{
					const auto v_abs     = (R)std::abs(VN_value);
					const auto tan_v_abs = std::tanh(v_abs * (R)0.5);
					const auto res       = (tan_v_abs != 0) ? (R)std::log(tan_v_abs) : std::numeric_limits<R>::min();
					const auto c_sign    = std::signbit((float)VN_value) ? -1 : 0;

					this->sign         ^= c_sign;
					this->sum          += res;
					this->values[VN_id] = res;
				}

			inline void end_check_node_in()
			{
			}

			// outcomming values from the check nodes into the variable nodes
			inline void begin_check_node_out(const int CN_id, const int CN_degree)
			{
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline R compute_check_node_out(const int VN_id, const R VN_value)
				{
					const auto v_sig = sign ^ (std::signbit((float)VN_value) ? -1 : 0);
					const auto exp   = (sum - values[VN_id] != 0) ? std::exp(sum - values[VN_id]) :
					                                                (R)1.0 - std::numeric_limits<R>::epsilon();
					const auto v_tan = (R)2.0 * std::atanh(exp);

					return (R)std::copysign(v_tan, v_sig);
				}

			inline void end_check_node_out()
			{
			}

		inline void end_ite()
		{
		}

	inline void end_decoding()
	{
		// if (this->n_ite != (this->ite -1))
		// 	-> early termination
	}
};
}
}

#endif /* UPDATE_RULE_LSPA_HPP */
