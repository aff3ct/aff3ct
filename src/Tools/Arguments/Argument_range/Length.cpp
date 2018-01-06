#include "Length.hpp"

using namespace aff3ct;
using namespace tools;

Length_range* Length(size_t min_size = 0, size_t max_size = 0)
{
	return new Length_range(min_size, max_size);
}