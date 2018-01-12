#ifndef ARGUMENT_TYPE_REAL_HPP_
#define ARGUMENT_TYPE_REAL_HPP_

#include <string>
#include <stdexcept>

#include "../Argument_type_limited.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = float, typename... Ranges>
class Real_type : public Argument_type_limited<T,Ranges...>
{
public:
	template <typename r, typename... R>
	Real_type(const r* range, const R*... ranges)
	: Argument_type_limited<T,Ranges...>("real number", range, ranges...)
	{ }

	Real_type()
	: Argument_type_limited<T,Ranges...>("real number")
	{ }

	virtual ~Real_type() {};

	virtual Real_type<T,Ranges...>* clone() const
	{
		auto* clone = new Real_type<T,Ranges...>();

		return dynamic_cast<Real_type<T,Ranges...>*>(this->clone_ranges(clone));
	}

	template <typename... NewRanges>
	Real_type<T, Ranges..., NewRanges...>*
	clone(NewRanges*... new_ranges)
	{
		auto* clone = new Real_type<T, Ranges..., NewRanges...>();

		this->clone_ranges(clone);

		clone->add_ranges(new_ranges...);

		return clone;
	}

	virtual T convert(const std::string& val) const
	{
		return (T)std::stof(val);
	}

	virtual void check(const std::string& val) const
	{
		T real_num;

		try
		{
			real_num = this->convert(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be a real number");
		}

		this->check_ranges(real_num);
	}
};

template <typename T = float, typename... Ranges>
Real_type<T,Ranges...>* Real(Ranges*... ranges)
{
	return new Real_type<T,Ranges...>(ranges...);
}


}
}
#endif /* ARGUMENT_TYPE_REAL_HPP_ */