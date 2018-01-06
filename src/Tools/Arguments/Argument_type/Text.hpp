#ifndef ARGUMENT_TYPE_TEXT_HPP_
#define ARGUMENT_TYPE_TEXT_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string, typename... Ranges>
class Text_type : public Argument_type_limited<T,Ranges...>
{
public:
	Text_type(const Ranges*... ranges)
	: Argument_type_limited<T,Ranges...>("text", ranges...)
	{ }

	virtual ~Text_type() {};

	virtual Text_type<T,Ranges...>* clone() const
	{
		auto clone = new Text_type<T,Ranges...>(*this);

		return dynamic_cast<Text_type<T,Ranges...>*>(this->clone_ranges(clone));
	}

	virtual T convert(const std::string& val) const
	{
		return val;
	}


	virtual void check(const std::string& val) const
	{
		auto str_val = this->convert(val);

		if (str_val.empty())
			throw std::runtime_error("shall be a text");

		this->check_ranges(str_val);
	}
};

template <typename T = std::string, typename... Ranges>
Text_type<T,Ranges...>* Text(Ranges*... ranges)
{
	return new Text_type<T,Ranges...>(ranges...);
}

}
}
#endif /* ARGUMENT_TYPE_TEXT_HPP_ */