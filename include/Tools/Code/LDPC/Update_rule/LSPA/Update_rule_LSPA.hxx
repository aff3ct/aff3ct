#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <type_traits>
#include <sstream>
#include <cassert>
#include <limits>
#include <cmath>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/LDPC/Update_rule/LSPA/Update_rule_LSPA.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Update_rule_LSPA<R>::Update_rule_LSPA(const unsigned max_check_node_degree)
: name("LSPA"), values(max_check_node_degree), sign(0), sum(0), n_ite(0), ite(0)
{
	if (max_check_node_degree == 0)
	{
		std::stringstream message;
		message << "'max_check_node_degree' has to greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!std::is_same<R, double>::value && !std::is_same<R, float>::value)
	{
		std::stringstream message;
		message << "The 'LSPA' update rule supports only 'float' or 'double' datatypes.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename R>
inline std::string Update_rule_LSPA<R>
::get_name() const
{
	return this->name;
}

template <typename R>
inline void Update_rule_LSPA<R>
::begin_decoding(const int n_ite)
{
	this->n_ite = n_ite;
}

template <typename R>
inline void Update_rule_LSPA<R>
::begin_ite(const int ite)
{
	this->ite = ite;
}

template <typename R>
inline void Update_rule_LSPA<R>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	assert(chk_degree <= (int)values.size());

	this->sign = 0;
	this->sum  = 0;
}

template <typename R>
inline void Update_rule_LSPA<R>
::compute_chk_node_in(const int var_id, const R var_val)
{
	const auto var_abs     = (R)std::abs(var_val);
	const auto tan_var_abs = std::tanh(var_abs * (R)0.5);
	const auto res         = (tan_var_abs != 0) ? (R)std::log(tan_var_abs) : std::numeric_limits<R>::min();
	const auto var_sign    = std::signbit((float)var_val) ? -1 : 0;

	this->sign          ^= var_sign;
	this->sum           += res;
	this->values[var_id] = res;
}

template <typename R>
inline void Update_rule_LSPA<R>
::end_chk_node_in()
{
}

template <typename R>
inline void Update_rule_LSPA<R>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
}

template <typename R>
inline R Update_rule_LSPA<R>
::compute_chk_node_out(const int var_id, const R var_val)
{
	      auto res_tmp = this->sum - this->values[var_id];
	           res_tmp = (res_tmp != (R)0) ? (R)std::exp(res_tmp) : (R)1.0 - std::numeric_limits<R>::epsilon();
	const auto res_abs = (R)2.0 * std::atanh(res_tmp);
	const auto res_sgn = this->sign ^ (std::signbit((float)var_val) ? -1 : 0);

	return (R)std::copysign(res_abs, res_sgn);
}

template <typename R>
inline void Update_rule_LSPA<R>
::end_chk_node_out()
{
}

template <typename R>
inline void Update_rule_LSPA<R>
::end_ite()
{
}

template <typename R>
inline void Update_rule_LSPA<R>
::end_decoding()
{
}
}
}
