#include "None.hpp"

using namespace aff3ct;
using namespace tools;

None_type
::None_type()
: Argument_type("")
{

}

None_type
::~None_type()
{

}

None_type* None_type
::clone() const
{
	return new None_type(*this);
}

void None_type
::check(const std::string& val) const
{

}

aff3ct::tools::None_type* aff3ct::tools::None()
{
	return new None_type();
}