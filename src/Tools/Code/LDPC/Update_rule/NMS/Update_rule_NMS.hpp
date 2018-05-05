#ifndef UPDATE_RULE_NMS_HPP
#define UPDATE_RULE_NMS_HPP

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

template <typename R>
inline R normalize(const R val, const float factor)
{
	     if (factor == 0.125f) return tools::div8<R>(val);
	else if (factor == 0.250f) return tools::div4<R>(val);
	else if (factor == 0.375f) return tools::div4<R>(val) + tools::div8<R>(val);
	else if (factor == 0.500f) return tools::div2<R>(val);
	else if (factor == 0.625f) return tools::div2<R>(val) + tools::div8<R>(val);
	else if (factor == 0.750f) return tools::div2<R>(val) + tools::div4<R>(val);
	else if (factor == 0.875f) return tools::div2<R>(val) + tools::div4<R>(val) + tools::div8<R>(val);
	else if (factor == 1.000f) return val;
	else
	{
		std::stringstream message;
		message << "'factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f ('factor' = "
		        << factor << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <>
inline float normalize(const float val, const float factor)
{
	return val * factor;
}

template <>
inline double normalize(const double val, const float factor)
{
	return val * (double)factor;
}

template <typename R = float>
class Update_rule_NMS // Normalized Min Sum
{
protected:
	const float normalize_factor;
	Update_rule_MS<R> MS;

public:
	Update_rule_NMS(const float normalize_factor)
	: normalize_factor(normalize_factor), MS()
	{
	}

	virtual ~Update_rule_NMS()
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
				MS.cste1 = normalize<R>(MS.min2, this->normalize_factor);
				MS.cste2 = normalize<R>(MS.min1, this->normalize_factor);
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

#endif /* UPDATE_RULE_NMS_HPP */
