#ifndef ARGUMENT_TYPE_REAL_HPP_
#define ARGUMENT_TYPE_REAL_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

template <typename T = float>
class Real : public Argument_type_limited<T>
{
public:
	Real(const std::vector<Argument_range<T>*>& ranges = {})
	: Argument_type_limited<T>("real number", ranges)
	{ }

	virtual ~Real() {};

	virtual Real<T>* clone() const
	{
		auto clone = new Real<T>(*this);

		return dynamic_cast<Real<T>*>(this->clone_ranges(clone));
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


}
}
#endif /* ARGUMENT_TYPE_REAL_HPP_ */