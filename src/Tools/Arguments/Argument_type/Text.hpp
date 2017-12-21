#ifndef ARGUMENT_TYPE_TEXT_HPP_
#define ARGUMENT_TYPE_TEXT_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = std::string>
class Text : public Argument_type_limited<T>
{
public:
	Text(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("text", ranges)
	{ }

	virtual ~Text() {};

	virtual Text<T>* clone() const
	{
		auto clone = new Text<T>(*this);

		return dynamic_cast<Text<T>*>(this->clone_ranges(clone));
	}

	virtual T convert(const std::string& val) const
	{
		return val;
	}


	virtual void check(const std::string& val) const
	{
		auto str_val = this->convert(val);

		if (val.empty())
			throw std::runtime_error("shall be a text");

		this->check_ranges(str_val);
	}
};

}
}
#endif /* ARGUMENT_TYPE_TEXT_HPP_ */