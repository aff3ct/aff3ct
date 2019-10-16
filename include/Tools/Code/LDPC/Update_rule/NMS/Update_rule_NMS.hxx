#include <algorithm>
#include <sstream>

#include "Tools/Math/utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Code/LDPC/Update_rule/NMS/Update_rule_NMS.hpp"

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

template <typename R>
Update_rule_NMS<R>
::Update_rule_NMS(const float normalize_factor)
: name("NMS"), normalize_factor(normalize_factor), MS()
{
}

template <typename R>
inline std::string Update_rule_NMS<R>
::get_name() const
{
	return this->name;
}

template <typename R>
inline void Update_rule_NMS<R>
::begin_decoding(const int n_ite)
{
	MS.begin_decoding(n_ite);
}

template <typename R>
inline void Update_rule_NMS<R>
::begin_ite(const int ite)
{
	MS.begin_ite(ite);
}

template <typename R>
inline void Update_rule_NMS<R>
::begin_chk_node_in(const int chk_id, const int chk_degree)
{
	MS.begin_chk_node_in(chk_id, chk_degree);
}

template <typename R>
inline void Update_rule_NMS<R>
::compute_chk_node_in(const int var_id, const R var_val)
{
	MS.compute_chk_node_in(var_id, var_val);
}

template <typename R>
inline void Update_rule_NMS<R>
::end_chk_node_in()
{
	MS.cst1 = normalize<R>(MS.min2, this->normalize_factor);
	MS.cst2 = normalize<R>(MS.min1, this->normalize_factor);
	MS.cst1 = std::max((R)0, MS.cst1);
	MS.cst2 = std::max((R)0, MS.cst2);
}

template <typename R>
inline void Update_rule_NMS<R>
::begin_chk_node_out(const int chk_id, const int chk_degree)
{
	MS.begin_chk_node_out(chk_id, chk_degree);
}

template <typename R>
inline R Update_rule_NMS<R>
::compute_chk_node_out(const int var_id, const R var_val)
{
	return MS.compute_chk_node_out(var_id, var_val);
}

template <typename R>
inline void Update_rule_NMS<R>
::end_chk_node_out()
{
	MS.end_chk_node_out();
}

template <typename R>
inline void Update_rule_NMS<R>
::end_ite()
{
	MS.end_ite();
}

template <typename R>
inline void Update_rule_NMS<R>
::end_decoding()
{
	MS.end_decoding();
}
}
}
