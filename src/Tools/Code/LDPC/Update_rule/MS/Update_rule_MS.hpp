#ifndef UPDATE_RULE_MS_HPP
#define UPDATE_RULE_MS_HPP

#include <cassert>
#include <limits>
#include <cmath>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Update_rule_MS // Min Sum
{
protected:
	int sign;
	R min1;
	R min2;
	R cst1;
	R cst2;
	int n_ite;
	int ite;

public:
	Update_rule_MS()
	: sign(0), min1(std::numeric_limits<R>::max()), min2(std::numeric_limits<R>::max()), cst1(0), cst2(0), n_ite(0), ite(0)
	{
	}

	virtual ~Update_rule_MS()
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
				this->sign = 0;
				this->min1 = std::numeric_limits<R>::max();
				this->min2 = std::numeric_limits<R>::max();
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline void compute_check_node_in(const int VN_id, const R VN_value)
				{
					const auto v_abs  = (R)std::abs(VN_value);
					const auto c_sign = std::signbit((float)VN_value) ? -1 : 0;
					const auto v_temp = min1;

					this->sign ^= c_sign;
					this->min1  = std::min(this->min1,          v_abs         );
					this->min2  = std::min(this->min2, std::max(v_abs, v_temp));
				}

			inline void end_check_node_in()
			{
				this->cste1 = (this->min2 < 0) ? 0 : this->min2;
				this->cste2 = (this->min1 < 0) ? 0 : this->min1;
			}

			// outcomming values from the check nodes into the variable nodes
			inline void begin_check_node_out(const int CN_id, const int CN_degree)
			{
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline R compute_check_node_out(const int VN_id, const R VN_value)
				{
					const auto v_abs = (R)std::abs(VN_value);
					const auto v_res = ((v_abs == this->min1) ? this->cste1 : this->cste2);
					const auto v_sig = sign ^ (std::signbit((float)VN_value) ? -1 : 0);

					return (R)std::copysign(v_res, v_sig);
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

#endif /* UPDATE_RULE_MS_HPP */
