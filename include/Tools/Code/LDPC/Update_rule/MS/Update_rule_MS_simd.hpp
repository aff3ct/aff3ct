/*!
 * \file
 * \brief Class tools::Update_rule_MS_simd.
 */
#ifndef UPDATE_RULE_MS_SIMD_HPP
#define UPDATE_RULE_MS_SIMD_HPP
#ifdef __cpp_aligned_new

#include <string>
#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename R       > class Update_rule_OMS_simd;
template <typename R, int N> class Update_rule_NMS_simd;

template <typename R = float>
class Update_rule_MS_simd // Min Sum
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Update_rule_OMS_simd<R  >;
	friend Update_rule_NMS_simd<R,0>;
	friend Update_rule_NMS_simd<R,1>;
	friend Update_rule_NMS_simd<R,2>;
	friend Update_rule_NMS_simd<R,3>;
	friend Update_rule_NMS_simd<R,4>;
	friend Update_rule_NMS_simd<R,5>;
	friend Update_rule_NMS_simd<R,6>;
	friend Update_rule_NMS_simd<R,7>;
	friend Update_rule_NMS_simd<R,8>;
#endif

protected:
	const std::string name;
	const mipp::Msk<mipp::N<R>()> false_msk;
	const mipp::Reg<R> zero;
	const mipp::Reg<R> max;
	mipp::Msk<mipp::N<R>()> sign;
	mipp::Reg<R> min1;
	mipp::Reg<R> min2;
	mipp::Reg<R> cst1;
	mipp::Reg<R> cst2;

	int n_ite;
	int ite;

public:
	Update_rule_MS_simd();

	virtual ~Update_rule_MS_simd() = default;

	inline std::string get_name() const;

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	inline void begin_decoding(const int n_ite);

	inline void begin_ite(const int ite);

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree);

	inline void compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val);

	inline void end_chk_node_in();

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree);

	inline mipp::Reg<R> compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val);

	inline void end_chk_node_out();

	inline void end_ite();

	inline void end_decoding();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_simd.hxx"
#endif

#endif
#endif /* UPDATE_RULE_MS_SIMD_HPP */
