#ifndef UPDATE_RULE_OMS_HPP
#define UPDATE_RULE_OMS_HPP

#include <cassert>
#include <limits>
#include <string>
#include <cmath>

#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"
#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS.hpp"
#include "Tools/Math/utils.h"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Update_rule_OMS // Offset Min Sum
{
protected:
	const std::string name;
	const R offset;
	Update_rule_MS<R> MS;

public:
	explicit Update_rule_OMS(const R offset)
	: name("OMS"), offset(offset), MS()
	{
	}

	virtual ~Update_rule_OMS()
	{
	}

	std::string get_name() const
	{
		return this->name;
	}

	inline void begin_decoding(const int n_ite)
	{
		MS.begin_decoding(n_ite);
	}

	inline void begin_ite(const int ite)
	{
		MS.begin_ite(ite);
	}

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree)
	{
		MS.begin_chk_node_in(chk_id, chk_degree);
	}

	inline void compute_chk_node_in(const int var_id, const R var_val)
	{
		MS.compute_chk_node_in(var_id, var_val);
	}

	inline void end_chk_node_in()
	{
		MS.cst1 = MS.min2 - this->offset;
		MS.cst2 = MS.min1 - this->offset;
		MS.cst1 = (MS.cst1 < 0) ? 0 : MS.cst1;
		MS.cst2 = (MS.cst2 < 0) ? 0 : MS.cst2;
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
		MS.begin_chk_node_out(chk_id, chk_degree);
	}

	inline R compute_chk_node_out(const int var_id, const R var_val)
	{
		return MS.compute_chk_node_out(var_id, var_val);
	}

	inline void end_chk_node_out()
	{
		MS.end_chk_node_out();
	}

	inline void end_ite()
	{
		MS.end_ite();
	}

	inline void end_decoding()
	{
		MS.end_decoding();
	}
};
}
}

#endif /* UPDATE_RULE_OMS_HPP */