#ifndef UPDATE_RULE_LSPA_HPP
#define UPDATE_RULE_LSPA_HPP

#include <sstream>
#include <cassert>
#include <vector>
#include <limits>
#include <string>
#include <cmath>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Update_rule_LSPA // Log Sum Product Algorithm
{
protected:
	const std::string name;
	std::vector<R> values;
	int sign;
	R   sum;
	int n_ite;
	int ite;

public:
	explicit Update_rule_LSPA(const unsigned max_check_node_degree)
	: name("LSPA"), values(max_check_node_degree), sign(0), sum(1), n_ite(0), ite(0)
	{
		if (max_check_node_degree == 0)
		{
			std::stringstream message;
			message << "'max_check_node_degree' has to greater than 0.";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	virtual ~Update_rule_LSPA()
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
		assert(chk_degree <= (int)values.size());

		this->sign = 0;
		this->sum  = 0;
	}

	inline void compute_chk_node_in(const int var_id, const R var_val)
	{
		const auto var_abs     = (R)std::abs(var_val);
		const auto tan_var_abs = std::tanh(var_abs * (R)0.5);
		const auto res         = (tan_var_abs != 0) ? (R)std::log(tan_var_abs) : std::numeric_limits<R>::min();
		const auto var_sign    = std::signbit((float)var_val) ? -1 : 0;

		this->sign          ^= var_sign;
		this->sum           += res;
		this->values[var_id] = res;
	}

	inline void end_chk_node_in()
	{
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
	}

	inline R compute_chk_node_out(const int var_id, const R var_val)
	{
		      auto res_tmp = this->sum - this->values[var_id];
		           res_tmp = (res_tmp != (R)1.0) ? (R)std::exp(res_tmp) : (R)1.0 - std::numeric_limits<R>::epsilon();
		const auto res_abs = (R)2.0 * std::atanh(res_tmp);
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

#endif /* UPDATE_RULE_LSPA_HPP */
