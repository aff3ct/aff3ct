#ifndef UPDATE_RULE_AMS_HPP
#define UPDATE_RULE_AMS_HPP

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

	std::string get_name() const
	{
		return this->name;
	}

	inline void begin_decoding(const int n_ite)
	{
		this->n_ite = n_ite;
	}

	inline void begin_ite(const int ite)
	{
		this->ite = ite;
	}

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree)
	{
		this->sign      = 0;
		this->min       = std::numeric_limits<R>::max();
		this->delta_min = std::numeric_limits<R>::max();
	}

	inline void compute_chk_node_in(const int var_id, const R var_val)
	{
		const auto var_abs = (R)std::abs(var_val);
		const auto var_sgn = std::signbit((float)var_val) ? -1 : 0;
		const auto tmp     = this->min;

		this->sign     ^= var_sgn;
		this->min       = std::min(this->min, var_abs);
		this->delta_min = MIN(this->delta_min, (var_abs == this->min) ? tmp : var_abs);
	}

	inline void end_chk_node_in()
	{
		this->delta     = std::max((R)0, MIN(this->delta_min, this->min));
		this->delta_min = std::max((R)0, this->delta_min);
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
	}

	inline R compute_chk_node_out(const int var_id, const R var_val)
	{
		const auto var_abs = (R)std::abs(var_val);
		      auto res_abs = ((var_abs == this->min) ? this->delta_min : this->delta);
		const auto res_sgn = this->sign ^ (std::signbit((float)var_val) ? -1 : 0);

		return (R)std::copysign(res_abs, res_sgn);
	}

	inline void end_chk_node_out()
	{
	}

	inline void end_ite()
	{
	}

	inline void end_decoding()
	{
	}
};
}
}

#endif /* UPDATE_RULE_AMS_HPP */
