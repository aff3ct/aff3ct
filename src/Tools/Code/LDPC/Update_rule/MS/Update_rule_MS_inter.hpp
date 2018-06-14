#ifndef UPDATE_RULE_MS_INTER_HPP
#define UPDATE_RULE_MS_INTER_HPP

#include <limits>
#include <string>
#include <mipp.h>

namespace aff3ct
{
namespace tools
{
template <typename R       > class Update_rule_OMS_inter;
template <typename R, int N> class Update_rule_NMS_inter;

template <typename R = float>
class Update_rule_MS_inter // Min Sum
{
	friend Update_rule_OMS_inter<R  >;
	friend Update_rule_NMS_inter<R,0>;
	friend Update_rule_NMS_inter<R,1>;
	friend Update_rule_NMS_inter<R,2>;
	friend Update_rule_NMS_inter<R,3>;
	friend Update_rule_NMS_inter<R,4>;
	friend Update_rule_NMS_inter<R,5>;
	friend Update_rule_NMS_inter<R,6>;
	friend Update_rule_NMS_inter<R,7>;
	friend Update_rule_NMS_inter<R,8>;

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
	Update_rule_MS_inter()
	: name("MS"), false_msk(false), zero((R)0), max(std::numeric_limits<R>::max()), sign(false), min1(max), min2(max),
	  cst1(zero), cst2(zero), n_ite(0), ite(0)
	{
	}

	virtual ~Update_rule_MS_inter()
	{
	}

	std::string get_name() const
	{
		return this->name;
	}

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	inline void begin_decoding(const int n_ite)
	{
		this->n_ite = n_ite;
	}

	inline void begin_ite(const int ite)
	{
		this->ite = ite;
	}

	// incoming values from the variable nodes into the check nodes
	inline void begin_chk_node_in(const int chk_id, const int chk_degree)
	{
		this->sign = this->false_msk;
		this->min1 = this->max;
		this->min2 = this->max;
	}

	inline void compute_chk_node_in(const int var_id, const mipp::Reg<R> var_val)
	{
		const auto var_abs  = mipp::abs(var_val);
		const auto var_sign = mipp::sign(var_val);
		const auto tmp      = this->min1;

		this->sign ^= var_sign;
		this->min1  = mipp::min(this->min1,           var_abs      );
		this->min2  = mipp::min(this->min2, mipp::max(var_abs, tmp));
	}

	inline void end_chk_node_in()
	{
		this->cst1 = mipp::blend(this->zero, this->min2, this->min2 < this->zero);
		this->cst2 = mipp::blend(this->zero, this->min1, this->min1 < this->zero);
	}

	// outcomming values from the check nodes into the variable nodes
	inline void begin_chk_node_out(const int chk_id, const int chk_degree)
	{
	}

	inline mipp::Reg<R> compute_chk_node_out(const int var_id, const mipp::Reg<R> var_val)
	{
		const auto var_abs = mipp::abs(var_val);
		      auto res_abs = mipp::blend(this->cst1, this->cst2, var_abs == this->min1);
		const auto res_sng = this->sign ^ mipp::sign(var_val);

		return mipp::copysign(res_abs, res_sng);
	}

	inline void end_chk_node_out()
	{
	}

	inline void end_ite()
	{
	}

	inline void end_decoding()
	{
	}
};
}
}

#endif /* UPDATE_RULE_MS_INTER_HPP */
