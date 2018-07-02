#ifndef UPDATE_RULE_NMS_INTER_HPP
#define UPDATE_RULE_NMS_INTER_HPP

#include <cassert>
#include <limits>
#include <string>
#include <cmath>

#include "Tools/Code/LDPC/Update_rule/MS/Update_rule_MS_inter.hpp"
#include "Tools/Math/utils.h"

namespace aff3ct
{
namespace tools
{

template <typename R, int F = 0> inline mipp::Reg<R> normalize(const mipp::Reg<R> val, const float factor)
{
	return val * mipp::Reg<R>((R)factor);
}
template <> inline mipp::Reg<int16_t> normalize<int16_t, 1>(const mipp::Reg<int16_t> v, const float f) { return (v >> 3);                       } // v * 0.125
template <> inline mipp::Reg<int8_t > normalize<int8_t , 1>(const mipp::Reg<int8_t > v, const float f) { return (v >> 3);                       }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 2>(const mipp::Reg<int16_t> v, const float f) { return            (v >> 2);            } // v * 0.250
template <> inline mipp::Reg<int8_t > normalize<int8_t , 2>(const mipp::Reg<int8_t > v, const float f) { return            (v >> 2);            }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 3>(const mipp::Reg<int16_t> v, const float f) { return (v >> 3) + (v >> 2);            } // v * 0.375
template <> inline mipp::Reg<int8_t > normalize<int8_t , 3>(const mipp::Reg<int8_t > v, const float f) { return (v >> 3) + (v >> 2);            }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 4>(const mipp::Reg<int16_t> v, const float f) { return                       (v >> 1); } // v * 0.500
template <> inline mipp::Reg<int8_t > normalize<int8_t , 4>(const mipp::Reg<int8_t > v, const float f) { return                       (v >> 1); }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 5>(const mipp::Reg<int16_t> v, const float f) { return (v >> 3) +            (v >> 1); } // v * 0.625
template <> inline mipp::Reg<int8_t > normalize<int8_t , 5>(const mipp::Reg<int8_t > v, const float f) { return (v >> 3) +            (v >> 1); }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 6>(const mipp::Reg<int16_t> v, const float f) { return            (v >> 2) + (v >> 1); } // v * 0.750
template <> inline mipp::Reg<int8_t > normalize<int8_t , 6>(const mipp::Reg<int8_t > v, const float f) { return            (v >> 2) + (v >> 1); }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 7>(const mipp::Reg<int16_t> v, const float f) { return (v >> 3) + (v >> 2) + (v >> 1); } // v * 0.825
template <> inline mipp::Reg<int8_t > normalize<int8_t , 7>(const mipp::Reg<int8_t > v, const float f) { return (v >> 3) + (v >> 2) + (v >> 1); }
template <> inline mipp::Reg<int16_t> normalize<int16_t, 8>(const mipp::Reg<int16_t> v, const float f) { return v;                              } // v * 1.000
template <> inline mipp::Reg<int8_t > normalize<int8_t , 8>(const mipp::Reg<int8_t > v, const float f) { return v;                              }
template <> inline mipp::Reg<float  > normalize<float  , 8>(const mipp::Reg<float  > v, const float f) { return v;                              }
template <> inline mipp::Reg<double > normalize<double , 8>(const mipp::Reg<double > v, const float f) { return v;                              }


template <typename R = float, int F = 0>
class Update_rule_NMS_inter // Normalized Min Sum
{
protected:
	const std::string name;
	const float normalize_factor;
	Update_rule_MS_inter<R> MS;

public:
	explicit Update_rule_NMS_inter(const float normalize_factor)
	: name("NMS"), normalize_factor(normalize_factor), MS()
	{
		if (sizeof(R) == 1)
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "This update rule does not work in 8-bit fixed-point.");

		if (typeid(R) == typeid(int16_t) || typeid(R) == typeid(int8_t))
		{
			bool error = false;
			     if (normalize_factor == 0.125f) { if (F != 1) error = true; }
			else if (normalize_factor == 0.250f) { if (F != 2) error = true; }
			else if (normalize_factor == 0.375f) { if (F != 3) error = true; }
			else if (normalize_factor == 0.500f) { if (F != 4) error = true; }
			else if (normalize_factor == 0.625f) { if (F != 5) error = true; }
			else if (normalize_factor == 0.750f) { if (F != 6) error = true; }
			else if (normalize_factor == 0.875f) { if (F != 7) error = true; }
			else if (normalize_factor == 1.000f) { if (F != 8) error = true; }
			else
			{
				std::stringstream message;
				message << "'normalize_factor' can only be 0.125f, 0.250f, 0.375f, 0.500f, 0.625f, 0.750f, 0.875f or 1.000f"
				        << " ('normalize_factor' = " << normalize_factor << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}

			if (error)
			{
				std::stringstream message;
				message << "Incompatible 'normalize_factor' and 'F' template ('normalize_factor' = "
				        << normalize_factor << ", 'F' = " << F << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
	}

	virtual ~Update_rule_NMS_inter()
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

	inline void compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
	{
		MS.compute_chk_node_in(var_id, var_val);
	}

	inline void end_chk_node_in()
	{
		MS.cst1 = normalize<R,F>(MS.min2, this->normalize_factor);
		MS.cst2 = normalize<R,F>(MS.min1, this->normalize_factor);
		MS.cst1 = mipp::blend(MS.zero, MS.cst1, MS.cst1 < MS.zero);
		MS.cst2 = mipp::blend(MS.zero, MS.cst2, MS.cst2 < MS.zero);
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
		MS.begin_chk_node_out(chk_id, chk_degree);
	}

	inline mipp::Reg<R> compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
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

#endif /* UPDATE_RULE_NMS_INTER_HPP */
