#include <limits>

#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_simd.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Update_rule_MS_simd<R>
::Update_rule_MS_simd()
: name("MS"), false_msk(false), zero((R)0), max(std::numeric_limits<R>::max()), sign(false), min1(max), min2(max),
  cst1(zero), cst2(zero), n_ite(0), ite(0)
{
}

template <typename R>
inline std::string Update_rule_MS_simd<R>
::get_name() const
{
	return this->name;
}

template <typename R>
inline void Update_rule_MS_simd<R>
::begin_decoding(const int n_ite)
{
	this->n_ite = n_ite;
}

template <typename R>
inline void Update_rule_MS_simd<R>
::begin_ite(const int ite)
{
	this->ite = ite;
}

template <typename R>
inline void Update_rule_MS_simd<R>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	this->sign = this->false_msk;
	this->min1 = this->max;
	this->min2 = this->max;
}

template <typename R>
inline void Update_rule_MS_simd<R>
::compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
{
	const auto var_abs  = mipp::abs(var_val);
	const auto var_sign = mipp::sign(var_val);

	this->sign ^= var_sign;
	this->min2  = mipp::min(this->min2, mipp::max(var_abs, this->min1));
	this->min1  = mipp::min(this->min1,           var_abs             );
}

template <typename R>
inline void Update_rule_MS_simd<R>
::end_chk_node_in()
{
	this->cst1 = mipp::max(this->zero, this->min2);
	this->cst2 = mipp::max(this->zero, this->min1);
}

template <typename R>
inline void Update_rule_MS_simd<R>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
}

template <typename R>
inline mipp::Reg<R> Update_rule_MS_simd<R>
::compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
{
	const auto var_abs = mipp::abs(var_val);
	      auto res_abs = mipp::blend(this->cst1, this->cst2, var_abs == this->min1);
	const auto res_sng = this->sign ^ mipp::sign(var_val);

	return mipp::copysign(res_abs, res_sng);
}

template <typename R>
inline void Update_rule_MS_simd<R>
::end_chk_node_out()
{
}

template <typename R>
inline void Update_rule_MS_simd<R>
::end_ite()
{
}

template <typename R>
inline void Update_rule_MS_simd<R>
::end_decoding()
{
}
}
}
