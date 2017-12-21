#ifndef ARGUMENT_TYPE_BOOLEAN_HPP_
#define ARGUMENT_TYPE_BOOLEAN_HPP_

#include <string>
#include <stdexcept>
#include <sstream>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = int> // shall not be of type "bool" because a val of (int 5) would be converted
                            // to (bool 1) and then can't be checked correctly as a boolean value 0 or 1
class Boolean : public Argument_type_limited<T>
{
public:
	Boolean()
	: Argument_type_limited<T>("boolean", {new Including_set<int>({0, 1})})
	{ }

	virtual ~Boolean() {};

	virtual Boolean<T>* clone() const
	{
		auto clone = new Boolean<T>(*this);

		return dynamic_cast<Boolean<T>*>(this->clone_ranges(clone));
	}

	virtual int convert(const std::string& val) const
	{
		return std::stoi(val);
	}

	virtual void check(const std::string& val) const
	{
		int bool_num;

		try
		{
			bool_num = this->convert(val);
		}
		catch(std::exception&)
		{
			throw std::runtime_error("shall be a boolean");
		}

		this->check_ranges(bool_num);
	}
};

}
}
#endif /* ARGUMENT_TYPE_BOOLEAN_HPP_ */