#include <limits>

#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS_simd.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R, proto_min_i<R> MIN>
Update_rule_AMS_simd<R,MIN>::Update_rule_AMS_simd()
: name("AMS"), false_msk(false), max(std::numeric_limits<R>::max()), zero((R)0), sign(false), min(max),
  delta_min(max), delta(max), n_ite(0), ite(0)
{
}

template <typename R, proto_min_i<R> MIN>
std::string Update_rule_AMS_simd<R,MIN>
::get_name() const
{
	return this->name;
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::begin_decoding(const int n_ite)
{
	this->n_ite = n_ite;
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::begin_ite(const int ite)
{
	this->ite = ite;
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	this->sign      = this->false_msk;
	this->min       = this->max;
	this->delta_min = this->max;
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
{
	const auto var_abs = mipp::abs(var_val);
	const auto var_sgn = mipp::sign(var_val);
	const auto tmp     = this->min;

	this->sign     ^= var_sgn;
	this->min       = mipp::min(this->min, var_abs);
	this->delta_min = MIN(this->delta_min, mipp::blend(tmp, var_abs, var_abs == this->min));
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::end_chk_node_in()
{
	this->delta     = mipp::max(zero, MIN(this->delta_min, this->min));
	this->delta_min = mipp::max(zero, this->delta_min);
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
}

template <typename R, proto_min_i<R> MIN>
mipp::Reg<R> Update_rule_AMS_simd<R,MIN>
::compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
{
	const auto var_abs = mipp::abs(var_val);
	      auto res_abs = mipp::blend(this->delta_min, this->delta, var_abs == this->min);
	const auto res_sgn = this->sign ^ mipp::sign(var_val);

	return mipp::copysign(res_abs, res_sgn);
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::end_chk_node_out()
{
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::end_ite()
{
}

template <typename R, proto_min_i<R> MIN>
void Update_rule_AMS_simd<R,MIN>
::end_decoding()
{
}
}
}
