/*!
 * \file
 * \brief Class tools::Update_rule_AMS_simd.
 */
#ifndef UPDATE_RULE_AMS_SIMD_HPP
#define UPDATE_RULE_AMS_SIMD_HPP
#ifdef __cpp_aligned_new

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
	Update_rule_AMS_simd();

	virtual ~Update_rule_AMS_simd() = default;

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
#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS_simd.hxx"
#endif

#endif
#endif /* UPDATE_RULE_AMS_SIMD_HPP */
