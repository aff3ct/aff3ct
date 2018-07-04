#include "Argument_map_value.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

bool Argument_map_value
::exist(const Argument_tag &tags) const
{
	return (this->find(tags) != this->end());
}

int Argument_map_value
::to_int(const Argument_tag &tags) const
{
	try
	{
		return std::stoi(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0;
	}
}

float Argument_map_value
::to_float(const Argument_tag &tags) const
{
	try
	{
		return std::stof(this->at(tags));
	}
	catch (std::exception&)
	{
		return 0.0f;
	}
}
