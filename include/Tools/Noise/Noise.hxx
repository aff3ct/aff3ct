#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Noise/Noise.hpp"

namespace aff3ct
{
namespace tools
{

template <typename R>
Noise_type Noise<R>
::str_to_type(const std::string& str)
{
	Noise_type t;

	if (str == "SIGMA")
		t = Noise_type::SIGMA;
	else if (str == "ROP")
		t = Noise_type::ROP;
	else if (str == "EP")
		t = Noise_type::EP;
	else
	{
		std::stringstream message;
		message << "The string 'str' does not represent a noise type ('str' = " << str << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return t;
}

template <typename R>
std::string Noise<R>
::type_to_str(Noise_type t)
{
	std::string str;

	switch(t)
	{
		case Noise_type::SIGMA:
			str = "SIGMA";
			break;
		case Noise_type::EP:
			str = "EP";
			break;
		case Noise_type::ROP:
			str = "ROP";
			break;
	}

	if (str.empty()) // this 'if' is a test outside the switch case (instead of default) to keep the compiler check that all
		// cases of 'Noise_type' are well represented.
	{
		std::stringstream message;
		message << "The type 't' does not represent a noise type ('t' = " << (int8_t)t << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	return str;
}
}
}
