#ifndef ARGUMENT_TYPE_NONE_HPP_
#define ARGUMENT_TYPE_NONE_HPP_

#include <string>
#include <stdexcept>

#include "Argument_type.hpp"

namespace aff3ct
{
namespace tools
{

class None_type : public Argument_type
{
public:
	None_type();

	virtual ~None_type();

	virtual None_type* clone() const;

	virtual void check(const std::string& val) const;
};

None_type* None();

}
}
#endif /* ARGUMENT_TYPE_NONE_HPP_ */