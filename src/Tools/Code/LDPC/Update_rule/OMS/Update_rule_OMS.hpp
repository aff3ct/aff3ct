#ifndef UPDATE_RULE_OMS_HPP
#define UPDATE_RULE_OMS_HPP

#include <cassert>
#include <limits>
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
	const R offset;
	Update_rule_MS<R> MS;

public:
	Update_rule_OMS(const R offset)
	: offset(offset), MS()
	{
	}

	virtual ~Update_rule_OMS()
	{
	}

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	inline void begin_decoding(const int n_ite)
	{
		MS.begin_decoding(n_ite);
	}

	// FOR EACH iterations --------------------------------------------------------------------------------------- LOOP

		inline void begin_ite(const int ite)
		{
			MS.begin_ite(ite);
		}

		// FOR EACH check nodes ---------------------------------------------------------------------------------- LOOP

			// incoming values from the variable nodes into the check nodes
			inline void begin_check_node_in(const int CN_id, const int CN_degree)
			{
				MS.begin_check_node_in(CN_id, CN_degree);
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline void compute_check_node_in(const int VN_id, const R VN_value)
				{
					MS.compute_check_node_in(VN_id, VN_value);
				}

			inline void end_check_node_in()
			{
				MS.cste1 = MS.min2 - this->offset;
				MS.cste2 = MS.min1 - this->offset;
				MS.cste1 = (MS.cste1 < 0) ? 0 : MS.cste1;
				MS.cste2 = (MS.cste2 < 0) ? 0 : MS.cste2;
			}

			// outcomming values from the check nodes into the variable nodes
			inline void begin_check_node_out(const int CN_id, const int CN_degree)
			{
				MS.begin_check_node_out(CN_id, CN_degree);
			}

			// FOR EACH variable nodes of the current check node ------------------------------------------------- LOOP

				inline R compute_check_node_out(const int VN_id, const R VN_value)
				{
					MS.compute_check_node_out(VN_id, VN_value);
				}

			inline void end_check_node_out()
			{
				MS.end_check_node_out();
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
