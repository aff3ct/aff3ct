#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <limits>
#include <cmath>

#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R, proto_min<R> MIN>
Update_rule_AMS<R,MIN>
::Update_rule_AMS()
: name("AMS"), sign(0), min(std::numeric_limits<R>::max()), delta_min(min), delta(min), n_ite(0), ite(0)
{
}

template <typename R, proto_min<R> MIN>
std::string Update_rule_AMS<R,MIN>
::get_name() const
{
	return this->name;
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::begin_decoding(const int n_ite)
{
	this->n_ite = n_ite;
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::begin_ite(const int ite)
{
	this->ite = ite;
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	this->sign      = 0;
	this->min       = std::numeric_limits<R>::max();
	this->delta_min = std::numeric_limits<R>::max();
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::compute_chk_node_in(const int var_id, const R var_val)
{
	const auto var_abs = (R)std::abs(var_val);
	const auto var_sgn = std::signbit((float)var_val) ? -1 : 0;
	const auto tmp     = this->min;

	this->sign     ^= var_sgn;
	this->min       = std::min(this->min, var_abs);
	this->delta_min = MIN(this->delta_min, (var_abs == this->min) ? tmp : var_abs);
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::end_chk_node_in()
{
	this->delta     = std::max((R)0, MIN(this->delta_min, this->min));
	this->delta_min = std::max((R)0, this->delta_min);
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
}

template <typename R, proto_min<R> MIN>
R Update_rule_AMS<R,MIN>
::compute_chk_node_out(const int var_id, const R var_val)
{
	const auto var_abs = (R)std::abs(var_val);
	      auto res_abs = ((var_abs == this->min) ? this->delta_min : this->delta);
	const auto res_sgn = this->sign ^ (std::signbit((float)var_val) ? -1 : 0);

	return (R)std::copysign(res_abs, res_sgn);
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::end_chk_node_out()
{
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::end_ite()
{
}

template <typename R, proto_min<R> MIN>
void Update_rule_AMS<R,MIN>
::end_decoding()
{
}
}
}
