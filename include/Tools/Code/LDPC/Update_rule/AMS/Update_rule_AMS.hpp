/*!
 * \file
 * \brief Class tools::Update_rule_AMS.
 */
#ifndef UPDATE_RULE_AMS_HPP
#define UPDATE_RULE_AMS_HPP

#include <string>

#include "Tools/Math/max.h"

namespace aff3ct
{
namespace tools
{
template <typename R = float, proto_min<R> MIN = min_star_linear2>
class Update_rule_AMS // Approximate Min Star
{
protected:
	const std::string name;
	int sign;
	R min;
	R delta_min;
	R delta;
	int n_ite;
	int ite;

public:
	Update_rule_AMS();

	virtual ~Update_rule_AMS() = default;

	inline std::string get_name() const;

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
#include "Tools/Code/LDPC/Update_rule/AMS/Update_rule_AMS.hxx"
#endif

#endif /* UPDATE_RULE_AMS_HPP */
