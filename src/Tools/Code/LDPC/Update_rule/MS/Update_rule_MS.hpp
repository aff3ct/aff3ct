#ifndef UPDATE_RULE_MS_HPP
#define UPDATE_RULE_MS_HPP

#include <cassert>
#include <limits>
#include <string>
#include <cmath>

namespace aff3ct
{
namespace tools
{
template <typename R> class Update_rule_OMS;
template <typename R> class Update_rule_NMS;

template <typename R = float>
class Update_rule_MS // Min Sum
{
	friend Update_rule_OMS<R>;
	friend Update_rule_NMS<R>;

protected:
	const std::string name;
	int sign;
	R min1;
	R min2;
	R cst1;
	R cst2;
	int n_ite;
	int ite;

public:
	Update_rule_MS()
	: name("MS"), sign(0), min1(std::numeric_limits<R>::max()), min2(min1), cst1(0), cst2(0), n_ite(0), ite(0)
	{
	}

	virtual ~Update_rule_MS()
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
		this->sign = 0;
		this->min1 = std::numeric_limits<R>::max();
		this->min2 = std::numeric_limits<R>::max();
	}

	inline void compute_chk_node_in(const int var_id, const R var_val)
	{
		const auto var_abs  = (R)std::abs(var_val);
		const auto var_sign = std::signbit((float)var_val) ? -1 : 0;

		this->sign ^= var_sign;
		this->min2  = std::min(this->min2, std::max(var_abs, this->min1));
		this->min1  = std::min(this->min1,          var_abs             );
	}

	inline void end_chk_node_in()
	{
		this->cst1 = std::max((R)0, this->min2);
		this->cst2 = std::max((R)0, this->min1);
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
	}

	inline R compute_chk_node_out(const int var_id, const R var_val)
	{
		const auto var_abs = (R)std::abs(var_val);
		const auto res_abs = ((var_abs == this->min1) ? this->cst1 : this->cst2);
		const auto res_sng = this->sign ^ (std::signbit((float)var_val) ? -1 : 0);

		return (R)std::copysign(res_abs, res_sng);
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

#endif /* UPDATE_RULE_MS_HPP */
