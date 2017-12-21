#ifndef ARGUMENT_TYPE_INTEGER_HPP_
#define ARGUMENT_TYPE_INTEGER_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = int>
class Integer : public Argument_type_limited<T>
{
public:
	Integer(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("integer", ranges)
	{ }

	virtual ~Integer() {};

	virtual Integer<T>* clone() const
	{
		auto clone = new Integer<T>(*this);

		return dynamic_cast<Integer<T>*>(this->clone_ranges(clone));
	}

	virtual T convert(const std::string& val) const
	{
		return (T)std::stoi(val);
	}

	virtual void check(const std::string& val) const
	{
		T int_num;

		try
		{
			int_num = this->convert(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be an integer");
		}

		this->check_ranges(int_num);
	}
};

}
}
#endif /* ARGUMENT_TYPE_INTEGER_HPP_ */