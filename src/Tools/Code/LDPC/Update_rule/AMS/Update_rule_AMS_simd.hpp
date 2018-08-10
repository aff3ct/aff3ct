#ifndef UPDATE_RULE_AMS_SIMD_HPP
#define UPDATE_RULE_AMS_SIMD_HPP

#include <vector>
#include <limits>
#include <string>
#include <mipp.h>

#include "Tools/Math/max.h"

namespace aff3ct
{
namespace tools
{
template <typename R = float, proto_min_i<R> MIN = min_star_linear2_i>
class Update_rule_AMS_simd // Approximate Min Star
{
protected:
	const std::string name;
	const mipp::Msk<mipp::N<R>()> false_msk;
	const mipp::Reg<R> max;
	const mipp::Reg<R> zero;
	mipp::Msk<mipp::N<R>()> sign;
	mipp::Reg<R> min;
	mipp::Reg<R> delta_min;
	mipp::Reg<R> delta;

	int n_ite;
	int ite;

public:
	Update_rule_AMS_simd()
	: name("AMS"), false_msk(false), max(std::numeric_limits<R>::max()), zero((R)0), sign(false), min(max),
	  delta_min(max), delta(max), n_ite(0), ite(0)
	{
	}

	virtual ~Update_rule_AMS_simd()
	{
	}

	std::string get_name() const
	{
		return this->name;
	}

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

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
		this->sign      = this->false_msk;
		this->min       = this->max;
		this->delta_min = this->max;
	}

	inline void compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
	{
		const auto var_abs = mipp::abs(var_val);
		const auto var_sgn = mipp::sign(var_val);
		const auto tmp     = this->min;

		this->sign     ^= var_sgn;
		this->min       = mipp::min(this->min, var_abs);
		this->delta_min = MIN(this->delta_min, mipp::blend(tmp, var_abs, var_abs == this->min));
	}

	inline void end_chk_node_in()
	{
		this->delta     = mipp::max(zero, MIN(this->delta_min, this->min));
		this->delta_min = mipp::max(zero, this->delta_min);
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
	}

	inline mipp::Reg<R> compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
	{
		const auto var_abs = mipp::abs(var_val);
		      auto res_abs = mipp::blend(this->delta_min, this->delta, var_abs == this->min);
		const auto res_sgn = this->sign ^ mipp::sign(var_val);

		return mipp::copysign(res_abs, res_sgn);
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

#endif /* UPDATE_RULE_AMS_SIMD_HPP */
