#ifndef UPDATE_RULE_AMS_HPP
#define UPDATE_RULE_AMS_HPP

#include <cassert>
#include <limits>
#include <string>
#include <cmath>

#include "Tools/Math/max.h"

namespace aff3ct
{
namespace tools
{
template <typename R = float, proto_min<R> MIN = min_star_linear2>
class Update_rule_AMS // Approximate Min Star
{
protected:
	const std::string name;
	int sign;
	R min;
	R delta_min;
	R delta;
	int n_ite;
	int ite;

public:
	Update_rule_AMS()
	: name("AMS"), sign(0), min(std::numeric_limits<R>::max()), delta_min(min), delta(min), n_ite(0), ite(0)
	{
	}

	virtual ~Update_rule_AMS()
	{
	}

	std::string get_name() { return this->name; }

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
				this->sign      = 0;
				this->min       = std::numeric_limits<R>::max();
				this->delta_min = std::numeric_limits<R>::max();
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline void compute_check_node_in(const int VN_id, const R VN_value)
				{
					const auto val_abs = (R)std::abs(VN_value);
					const auto val_sgn = std::signbit((float)VN_value) ? -1 : 0;
					const auto tmp     = this->min;

					this->sign     ^= val_sgn;
					this->min       = std::min(this->min, val_abs);
					this->delta_min = MIN(this->delta_min, (val_abs == this->min) ? tmp : val_abs);
				}

			inline void end_check_node_in()
			{
				this->delta     = std::max((R)0, MIN(this->delta_min, this->min));
				this->delta_min = std::max((R)0, this->delta_min);
			}

			// outcomming values from the check nodes into the variable nodes
			inline void begin_check_node_out(const int CN_id, const int CN_degree)
			{
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline R compute_check_node_out(const int VN_id, const R VN_value)
				{
					const auto val_abs = (R)std::abs(VN_value);
					      auto res_abs = ((val_abs == this->min) ? this->delta_min : this->delta);
					const auto res_sgn = this->sign ^ (std::signbit((float)VN_value) ? -1 : 0);

					return (R)std::copysign(res_abs, res_sgn);
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

#endif /* UPDATE_RULE_AMS_HPP */
