#include "Tools/Code/LDPC/Update_rule/OMS/Update_rule_OMS_simd.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R>
Update_rule_OMS_simd<R>
::Update_rule_OMS_simd(const R offset)
: name("OMS"), offset(offset), MS()
{
}

template <typename R>
inline std::string Update_rule_OMS_simd<R>
::get_name() const
{
	return this->name;
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::begin_decoding(const int n_ite)
{
	MS.begin_decoding(n_ite);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::begin_ite(const int ite)
{
	MS.begin_ite(ite);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	MS.begin_chk_node_in(chk_id, chk_degree);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
{
	MS.compute_chk_node_in(var_id, var_val);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::end_chk_node_in()
{
	MS.cst1 = MS.min2 - this->offset;
	MS.cst2 = MS.min1 - this->offset;
	MS.cst1 = mipp::max(MS.zero, MS.cst1);
	MS.cst2 = mipp::max(MS.zero, MS.cst2);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
	MS.begin_chk_node_out(chk_id, chk_degree);
}

template <typename R>
inline mipp::Reg<R> Update_rule_OMS_simd<R>
::compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
{
	return MS.compute_chk_node_out(var_id, var_val);
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::end_chk_node_out()
{
	MS.end_chk_node_out();
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::end_ite()
{
	MS.end_ite();
}

template <typename R>
inline void Update_rule_OMS_simd<R>
::end_decoding()
{
	MS.end_decoding();
}
}
}
