#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <algorithm>
#include <limits>
#include <cmath>

#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Update_rule_MS<R>
::Update_rule_MS()
: name("MS"), sign(0), min1(std::numeric_limits<R>::max()), min2(min1), cst1(0), cst2(0), n_ite(0), ite(0)
{
}

template <typename R>
inline std::string Update_rule_MS<R>
::get_name() const
{
	return this->name;
}

template <typename R>
inline void Update_rule_MS<R>
::begin_decoding(const int n_ite)
{
	this->n_ite = n_ite;
}

template <typename R>
inline void Update_rule_MS<R>
::begin_ite(const int ite)
{
	this->ite = ite;
}

template <typename R>
inline void Update_rule_MS<R>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	this->sign = 0;
	this->min1 = std::numeric_limits<R>::max();
	this->min2 = std::numeric_limits<R>::max();
}

template <typename R>
inline void Update_rule_MS<R>
::compute_chk_node_in(const int var_id, const R var_val)
{
	const auto var_abs  = (R)std::abs(var_val);
	const auto var_sign = std::signbit((float)var_val) ? -1 : 0;

	this->sign ^= var_sign;
	this->min2  = std::min(this->min2, std::max(var_abs, this->min1));
	this->min1  = std::min(this->min1,          var_abs             );
}

template <typename R>
inline void Update_rule_MS<R>
::end_chk_node_in()
{
	this->cst1 = std::max((R)0, this->min2);
	this->cst2 = std::max((R)0, this->min1);
}

template <typename R>
inline void Update_rule_MS<R>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
}

template <typename R>
inline R Update_rule_MS<R>
::compute_chk_node_out(const int var_id, const R var_val)
{
	const auto var_abs = (R)std::abs(var_val);
	const auto res_abs = ((var_abs == this->min1) ? this->cst1 : this->cst2);
	const auto res_sng = this->sign ^ (std::signbit((float)var_val) ? -1 : 0);

	return (R)std::copysign(res_abs, res_sng);
}

template <typename R>
inline void Update_rule_MS<R>
::end_chk_node_out()
{
}

template <typename R>
inline void Update_rule_MS<R>
::end_ite()
{
}

template <typename R>
inline void Update_rule_MS<R>
::end_decoding()
{
}
}
}
