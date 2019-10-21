/*!
 * \file
 * \brief Class tools::Update_rule_MS.
 */
#ifndef UPDATE_RULE_MS_HPP
#define UPDATE_RULE_MS_HPP

#include <string>

namespace aff3ct
{
namespace tools
{
template <typename R> class Update_rule_OMS;
template <typename R> class Update_rule_NMS;

template <typename R = float>
class Update_rule_MS // Min Sum
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	friend Update_rule_OMS<R>;
	friend Update_rule_NMS<R>;
#endif

protected:
	const std::string name;
	int sign;
	R min1;
	R min2;
	R cst1;
	R cst2;
	int n_ite;
	int ite;

public:
	Update_rule_MS();

	virtual ~Update_rule_MS() = default;

	inline std::string get_name() const;

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	inline void begin_decoding(const int n_ite);

	inline void begin_ite(const int ite);

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree);

	inline void compute_chk_node_in(const int var_id, const R var_val);

	inline void end_chk_node_in();

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree);

	inline R compute_chk_node_out(const int var_id, const R var_val);

	inline void end_chk_node_out();

	inline void end_ite();

	inline void end_decoding();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hxx"
#endif

#endif /* UPDATE_RULE_MS_HPP */
