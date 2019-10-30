/*!
 * \file
 * \brief Class tools::Update_rule_NMS_simd.
 */
#ifndef UPDATE_RULE_NMS_SIMD_HPP
#define UPDATE_RULE_NMS_SIMD_HPP
#ifdef __cpp_aligned_new

#include <string>
#include <mipp.h>

#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_simd.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float, int F = 0>
class Update_rule_NMS_simd // Normalized Min Sum
{
protected:
	const std::string name;
	const float normalize_factor;
	Update_rule_MS_simd<R> MS;

public:
	explicit Update_rule_NMS_simd(const float normalize_factor);

	virtual ~Update_rule_NMS_simd() = default;

	inline std::string get_name() const;

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
#include "Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS_simd.hxx"
#endif

#endif
#endif /* UPDATE_RULE_NMS_SIMD_HPP */
